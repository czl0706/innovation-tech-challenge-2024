import streamlit as st
import sqlite3
import pandas as pd
import numpy as np
from datetime import datetime
import pytz
import ast
import plotly.graph_objects as go

# 數據庫設置
DB_NAME = "mqtt_data.db"

def get_all_timestamps():
    conn = sqlite3.connect(DB_NAME)
    c = conn.cursor()
    c.execute("SELECT DISTINCT timestamp FROM batch_data ORDER BY timestamp DESC")
    timestamps = [row[0] for row in c.fetchall()]
    conn.close()
    return timestamps

def get_data_by_timestamp(timestamp):
    conn = sqlite3.connect(DB_NAME)
    query = """
    SELECT * FROM batch_data WHERE timestamp = ?
    """
    df = pd.read_sql_query(query, conn, params=(timestamp,))
    conn.close()
    return df

def get_lng_history():
    conn = sqlite3.connect(DB_NAME)
    query = """
    SELECT timestamp, lng FROM batch_data ORDER BY timestamp
    """
    df = pd.read_sql_query(query, conn)
    conn.close()
    df['timestamp'] = pd.to_datetime(df['timestamp']).dt.tz_localize('UTC').dt.tz_convert('Asia/Taipei')
    return df

def plot_lng_history(df):
    fig = go.Figure()
    # fig.add_trace(go.Scatter(x=df['timestamp'], y=df['lng'], mode='lines+markers', name='LNG'))
    fig.add_trace(go.Scatter(x=np.arange(len(df['lng'])), y=df['lng'], mode='lines+markers', name='LNG'))
    
    # fig.update_layout(
    #     # title='LNG(搖晃總長) 歷史記錄',
    #     # title={
    #     #     'text': 'LNG 歷史記錄',
    #     #     'font': {'size': 24}  # 增大標題字體大小
    #     # },
    #     xaxis_title='測驗時間',
    #     yaxis_title='LNG (m)',
    #     hovermode='x unified',
    #     margin=dict(t=10, b=10, l=10, r=10)
    # )
    
    fig.update_layout(
        # title='LNG(搖晃總長) 歷史記錄',
        # title={
        #     'text': 'LNG 歷史記錄',
        #     'font': {'size': 24}  # 增大標題字體大小
        # },
        xaxis_title='',
        yaxis_title='LNG (m)',
        hovermode='x unified',
        margin=dict(t=10, b=10, l=10, r=10)
    )
    st.plotly_chart(fig)

def convert_to_local_time(timestamp):
    utc_time = datetime.strptime(timestamp, '%Y-%m-%d %H:%M:%S')
    utc_time = utc_time.replace(tzinfo=pytz.UTC)
    local_tz = pytz.timezone('Asia/Taipei')
    local_time = utc_time.astimezone(local_tz)
    return local_time.strftime('%Y-%m-%d %H:%M:%S')

def plot_raw_data(raw_data, selected_indices, channel_names):
    fig = go.Figure()
    for i in selected_indices:
        x = np.arange(raw_data.shape[0]) * 0.01
        fig.add_trace(go.Scatter(x=x, y=raw_data[:, i], mode='lines', name=channel_names[i]))
    fig.update_layout(xaxis_title='時間(s)', 
                      margin=dict(t=10, b=10, l=10, r=10),  # 減少邊距
                      yaxis_title='加速度(m/s²)')
    
    fig.update_layout(yaxis_range=[-5, 5])
    st.plotly_chart(fig)

def plot_prediction_data(pred_data):
    fig = go.Figure()
    for i in range(pred_data.shape[0]):
        fig.add_trace(go.Scatter(y=pred_data[:, i], mode='lines+markers', name=f'Prediction {i+1}'))
    fig.update_layout(title='Prediction Data', 
                      xaxis_title='Sample', 
                      margin=dict(t=10, b=10, l=10, r=10),
                      yaxis_title='Value')
    st.plotly_chart(fig)

def main():
    st.title("資料檢視平台")
    
    # 獲取並顯示 LNG 歷史記錄
    lng_history = get_lng_history()
    col1, col2 = st.columns(2)
    
    with col1:
        st.subheader("LNG(搖晃總長) 趨勢圖")
    with col2:
        st.markdown(
            """
            <style>
            .stButton > button {
                float: right;
            }
            </style>
            """,
            unsafe_allow_html=True
        )
        if st.button('刷新頁面'):
            st.rerun()

    plot_lng_history(lng_history)
    # 獲取所有時間戳
    timestamps = get_all_timestamps()
    local_timestamps = [convert_to_local_time(ts) for ts in timestamps]
    
    st.subheader("歷史記錄")

    # 創建下拉選單
    selected_local_time = st.selectbox("選擇測驗時間", local_timestamps)
    
    # 將本地時間轉換回 UTC 時間以檢索數據
    selected_utc_time = timestamps[local_timestamps.index(selected_local_time)]

    # 獲取選定時間戳的數據
    df = get_data_by_timestamp(selected_utc_time)

    if df.empty:
        st.warning("No data found for the selected timestamp.")
    else:
        # st.subheader("Data for " + selected_local_time)

        # 顯示基本信息
        # st.write(f"Batch ID: {df['batch_id'].values[0]}")
        # st.subheader("本次測驗數據")
        
        # st.write('\n\n'.join([
        #     f"LNG(搖晃總長): {df['lng'].values[0]: .2f}m",
        #     f"EA(搖晃包含面積): {df['ea'].values[0]: .2f}m^2",
        #     f"RMS(搖晃平方平均數): {df['rms'].values[0]: .2f}m/s^2"
        # ]))
        
        # 轉換原始數據和預測數據
        raw_data = np.array(ast.literal_eval(df['raw_data'].values[0]))
        pred_data = np.array(ast.literal_eval(df['pred_data'].values[0]))
        
        data = pred_data[:, 0]
        score = (np.sum(data == 2) * 0.3 + np.sum(data == 1) * 0.6 + np.sum(data == 0) * 1.2) / 24 * 100
        
        test_eval = pred_data[:, 1]
        test_eval = test_eval[pred_data[:, 0] == 2]
        
        unique, counts = np.unique(test_eval, return_counts=True)
        eval_dict = dict(zip(unique, counts))
        
        for i in range(2):
            if i not in eval_dict:
                eval_dict[i] = 0
                
        col1, col2, col3, col4 = st.columns(4)

        # 在每列中顯示一個指標
        with col1:
            st.metric(
                label="LNG(搖晃總長)",
                value=f"{df['lng'].values[0]:.2f} m"
            )

        with col2:
            st.metric(
                label="EA(搖晃包含面積)",
                value=f"{df['ea'].values[0]:.2f} m²"
            )

        with col3:
            st.metric(
                label="搖晃加速度之RMS",
                value=f"{df['rms'].values[0]:.2f} m/s²"
            )
        
        with col4:
            st.metric(
                label=f"測驗結果: {score:.1f}/120分",
                value=f"{'極好' if score > 95 else '不錯' if score > 65 else '待加強'}",
            )
        
        # st.subheader("結果詳細資訊")
        st.markdown(f"#### 結果詳細資訊")
        
        info_back = """
對於站立姿勢不穩且背部肌肉發展欠佳的嬰幼兒，可以考慮以下建議來幫助他們改善情況：

###### 加強背部肌肉的鍛煉

1. **俯臥撐訓練**：
   - **Tummy Time**：讓嬰幼兒俯臥在地上，這有助於鍛煉背部、頸部和肩膀的肌肉。
   - **Superman Pose**：在嬰幼兒俯臥時，引導他們抬起手臂和腿部，就像在做超人飛行的動作。

2. **四足位訓練**：
   - **Crawling**：鼓勵嬰幼兒四肢著地爬行，這有助於發展背部和核心肌肉。
   - **Bird-Dog Exercise**：在四足位上，讓嬰幼兒抬起對側的手臂和腿，然後換邊，這有助於增強背部和核心穩定性。

3. **坐姿鍛煉**：
   - **Supported Sitting**：讓嬰幼兒坐在你身前，然後你輕輕地牽著他的手，幫助他維持坐姿，這有助於鍛煉背部肌肉。
   - **Swiss Ball Sitting**：讓嬰幼兒坐在健身球上，輕輕地晃動球，這可以增加核心和背部肌肉的活動。

###### 改善站立姿勢的策略

1. **支撐和輔助設備**：
   - **Standers**：使用專業的站立輔助設備，幫助嬰幼兒維持正確的站立姿勢。
   - **Walkers**：讓嬰幼兒使用助行器，以提供更多的支撐和穩定性。

2. **姿勢矯正**：
   - **Postural Support Garments**：考慮使用姿勢支撐衣物或矯正帶，幫助嬰幼兒維持良好的姿勢。
                """
        
        info_belly = """
對於腹部肌肉發展欠佳的嬰幼兒，可以考慮以下建議來幫助他們增強腹部肌肉，並改善整體穩定性：

###### 加強腹部肌肉的鍛煉

1. **俯臥撐訓練**：
   - **Tummy Time**：每天進行幾次俯臥訓練，這不僅有助於背部肌肉的發展，還能鍛煉腹部肌肉。
   - **Reach and Roll**：在俯臥時，引導嬰幼兒伸手去拿玩具，這有助於激活腹部肌肉。

2. **坐姿鍛煉**：
   - **Supported Sitting**：在嬰幼兒坐姿時，讓他們玩一些需要前傾的遊戲，比如拿取放在前方的玩具，這有助於鍛煉腹部肌肉。
   - **Rolling Exercises**：鼓勵嬰幼兒從仰臥到俯臥進行翻滾，這過程需要腹部肌肉的參與。

3. **仰臥訓練**：
   - **Leg Lifts**：在仰臥時，輕輕地幫助嬰幼兒抬起腿部，這有助於加強腹部肌肉。
   - **Bicycle Legs**：讓嬰幼兒仰臥，然後輕輕地幫助他們進行腳踏車動作，這有助於激活腹部和腿部肌肉。
                """
        
        detail = ''
        if score > 95:
            detail += "**很好，繼續保持!**"
        elif score > 65:
            detail += '\n'.join([
                "**不錯，有進步空間！**",
                "以下有幾點建議",
                "- 避免長時間站立: 不要長時間站立，給他們提供足夠的坐下和休息時間。",
                "- 減少單腳站立: 避免單腳站立或將重心放在一隻腳上，這會增加不穩定性。",
                "- 減少極端姿勢: 避免進行極端的彎腰或仰頭動作，這可能會影響平衡。",
                "- 避免突然的改變姿勢: 避免突然的改變姿勢，例如快速從坐到站或反之亦然。",
                "- 提供支撐: 在嬰幼兒站立或行走時提供支撐，例如牽著手或使用助行器。",
                "- 避免光滑表面: 確保嬰幼兒在穩固的地面上行走或站立，避免光滑的地板。",
                "- 穿著合適的鞋子: 穿著適合的鞋子，提供足夠的支撐和防滑功能。",
            ])
        else:
            if np.abs(eval_dict[0] - eval_dict[1]) < 3:
                weak_part = '兩側肌肉'
                info = info_back + '\n\n' + info_belly
            elif eval_dict[0] > eval_dict[1]:
                weak_part = '腹部'
                info = info_belly
            else:
                weak_part = '背部'
                info = info_back
                
            detail += '\n'.join([
                f"**待加強，尤其是{weak_part}，以下有幾點建議！**",
                info
            ])
            
        st.markdown(detail)
        # st.write(f"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
                       

        # 可視化選項
        # st.subheader("原始資料顯示")
        st.markdown(f"#### 資料顯示")
        
        col1, col2 = st.columns(2)
        with col1:
            show_pred_data = st.checkbox("(機器學習)預測結果", value=True)
        with col2:
            show_raw_data = st.checkbox("加速度", value=True)
            

        if show_pred_data:
            col1, col2 = st.columns(2)
            
            with col1:
                st.markdown(f"##### 預測結果")
                
                # 計算每個類別的數量
                unique, counts = np.unique(pred_data[:, 0], return_counts=True)
                data_dict = dict(zip(unique, counts))

                # 確保所有類別都存在，如果不存在則設為0
                for i in range(3):
                    if i not in data_dict:
                        data_dict[i] = 0

                colors = ['#1A6BC3', '#8DC9FB', '#F6AEAD']
                # 創建圓餅圖
                fig = go.Figure(data=[go.Pie(
                    labels=['良好', '欠佳', '晃動'],
                    values=[data_dict[0], data_dict[1], data_dict[2]],
                    marker=dict(colors=colors)
                )])

                # 設置圖表標題
                fig.update_layout(margin=dict(t=10, b=10, l=10, r=10))

                # 在Streamlit中顯示圖表
                st.plotly_chart(fig)
            
            with col2:                    
                st.markdown(f"##### 姿勢評估")
                
                if len(test_eval) == 0 or score > 95:
                    st.write("姿勢良好，請繼續保持！")
                else:
                    # unique, counts = np.unique(test_eval, return_counts=True)
                    # data_dict = dict(zip(unique, counts))
                    
                    # for i in range(2):
                    #     if i not in data_dict:
                    #         data_dict[i] = 0
                    
                    fig = go.Figure(data=[go.Pie(
                        labels=['腹部不穩', '背部不穩'],
                        values=[eval_dict[0], eval_dict[1]]
                    )])
                    
                    # fig.update_layout(title='姿勢評估')
                    fig.update_layout(margin=dict(t=10, b=10, l=10, r=10))
                    st.plotly_chart(fig)
                
                # st.markdown(f"##### 前傾/後傾")
                
                
        if show_raw_data:
            # st.subheader("加速度")
            # st.markdown(f"##### 加速度")
            channels = {
                'acc_x': 0,
                'acc_y': 1
            }
            selected_channels = st.multiselect("加速度方向", list(channels.keys()), default=list(channels.keys()))
            selected_indices = [channels[ch] for ch in selected_channels]
            plot_raw_data(raw_data, selected_indices, ['acc_x', 'acc_y'])

            
            
        # # 顯示原始數據表格
        # if st.checkbox("Show Raw Data Table"):
        #     st.subheader("Raw Data Table")
        #     st.write(pd.DataFrame(raw_data, columns=[f"Channel {i+1}" for i in range(raw_data.shape[1])]))

        # # 顯示預測數據表格
        # if st.checkbox("Show Prediction Data Table"):
        #     st.subheader("Prediction Data Table")
        #     st.write(pd.DataFrame(pred_data, columns=[f"Prediction {i+1}" for i in range(pred_data.shape[1])]))

if __name__ == "__main__":
    main()