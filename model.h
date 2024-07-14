const float conv1_weight[120] = {-0.75919574, -0.56070334, -0.6772195, -0.43543416, -0.7840314, -0.10910838, -0.35748303, -0.2012855, -0.19854116, -0.32219106, -0.12684065, -0.43390712, -0.18593524, -0.39014047, -0.3718955, -0.1629404, -0.058654036, -0.098261476, -0.09605466, 0.012051335, -0.3139662, 0.065014794, -0.17712975, -0.19488257, 0.10916725, 0.01783018, 0.03997866, -0.058188543, -0.16555141, -0.25531772, 0.92899317, 1.1416858, 1.2058861, 1.1211286, 1.0588515, 0.56168187, 0.69631106, 0.5802197, 0.357058, 0.34053385, 0.73215234, 0.68828756, 0.869083, 0.7730715, 0.3340648, -0.06738021, 0.028540358, 0.053255077, -0.041567937, 0.05732607, 0.09967293, -0.083538696, -0.39261696, -0.06798535, -0.15158293, 0.014437993, 0.15276963, -0.1986908, -0.04456331, 0.190195, 0.4026267, 0.40975726, 0.49583417, 0.44938302, 0.32249725, 1.2134286, 1.46114, 1.2379109, 1.3255069, 1.4253799, 0.5119611, 0.7764675, 0.87780666, 0.9102088, 0.9777309, 0.12441614, 0.109042905, 0.2133736, -0.019641206, 0.17166649, -0.09762226, -0.19013354, -0.105868444, 0.17896149, 0.040414706, -0.020179268, 0.036072996, 0.04531527, -0.049918413, 0.063563764, -0.8945209, -1.0184103, -0.9080217, -0.9962243, -0.7935454, -0.990982, -0.9579329, -1.071413, -1.0058815, -1.1617497, -0.90076244, -0.98862153, -0.85411, -0.9006683, -0.9527658, 0.12594707, 0.15082534, 0.1551249, 0.02704585, 0.09074879, -0.14767753, 0.07039951, -0.1340416, -0.09145769, -0.10282049, 0.14936069, 0.027455421, -0.18248834, 0.091151536, 0.21031995};
const float conv1_bias[4] = {-0.08535966, 0.35858175, 0.14405377, 0.31854263};
const float conv2_weight[72] = {-0.0038484354, 0.06382009, -0.11935635, 0.41190904, 0.17836465, 0.20091578, -0.018520249, 0.03337007, 0.21447137, -0.06775134, -0.2864135, 0.042291954, -0.028965969, 0.2929395, 0.21045993, -0.2686551, -0.36824366, -0.39795965, 0.01699467, 0.12434879, 0.2697295, -0.020845849, 0.3154429, 0.24663395, 0.23355675, 0.27387145, 0.29996565, 0.24030906, 0.042514876, 0.34980062, -0.22081766, -0.09943543, -0.08277699, 0.36941695, 0.24604051, 0.06594007, 0.03818607, -0.18751921, 0.15765646, 0.097863935, 0.20358552, 0.10561835, -0.1459367, 0.04281901, -0.30285475, -0.06262396, 0.032126103, 0.22675912, 0.28617296, 0.24756624, 0.24310094, 0.014940222, 0.27434596, 0.06994881, 0.27636915, 0.17317057, 0.05216029, 0.045138314, 0.016230423, -0.007362977, -0.2631492, 0.19789846, 0.040294684, 0.14901264, 0.3195855, 0.23421593, 0.09652725, 0.24331777, 0.2227934, -0.13115004, -0.25015312, -0.17761278};
const float conv2_bias[6] = {-0.10182041, 0.23361121, -0.1622351, 0.842908, -0.30380276, 0.6797535};
const float fc1_weight[720] = {-0.19188923, -0.23783766, -0.22636357, -0.22843485, -0.2296315, -0.24053608, -0.0928146, -0.092806734, -0.07303236, -0.1488418, -0.13259919, -0.06555323, -0.09004845, -0.104313105, -0.06634138, -0.04247893, -0.06441531, -0.11220553, -0.119365, -0.10065635, -0.10301576, 0.030203275, -0.014202082, -0.014004421, -0.015603622, 0.07157797, -0.06542605, 0.0044437987, -0.16276564, -0.26698974, -0.028024126, -0.031751513, 0.07575034, -0.011660514, -0.017234856, -0.012628908, -0.026731653, -0.062195543, -0.048930362, -0.02959919, -0.025117932, -0.0439877, -0.098454885, -0.15956967, -0.10676494, -0.123316355, -0.06863895, -0.11980426, -0.07843383, -0.14907521, -0.09076735, -0.06860312, -0.02763073, -0.08226753, -0.0943815, -0.19577725, -0.12747736, -0.12785453, -0.16335444, 0.14546478, -0.29507273, -0.30132818, -0.2388236, -0.2682583, -0.2632975, -0.27805477, -0.1821773, -0.19309454, -0.24373785, -0.20860009, -0.22204004, -0.23380254, -0.2195831, -0.19746315, -0.17290649, -0.15269779, -0.20245355, -0.23907802, -0.20292306, -0.20175347, -0.12061829, -0.15063255, -0.1627654, -0.16748002, -0.15719318, -0.23150913, -0.20797516, -0.26099202, -0.26435775, -0.38735366, 0.1275588, 0.15077105, 0.12598044, 0.12141453, 0.139595, 0.11920517, 0.12809062, 0.20321909, 0.19827123, 0.1600935, 0.14223804, 0.18338574, 0.19839944, 0.12834753, 0.23537458, 0.28219137, 0.24859239, 0.25529745, 0.24521169, 0.3095682, 0.28407565, 0.21642378, 0.18219608, 0.26888186, 0.2742651, 0.2944707, 0.20266819, 0.18747434, 0.23027207, 0.34315342, -0.29259256, -0.18515071, -0.17102908, -0.16449176, -0.15436727, -0.12808059, -0.16315955, -0.118687466, -0.22051708, -0.13603328, -0.20662285, -0.16760932, -0.14798042, -0.1370924, -0.20554416, -0.15797096, -0.13471499, -0.21189395, -0.10321329, -0.18392883, -0.19559942, -0.18038616, -0.18285257, -0.20846014, -0.20168418, -0.23872222, -0.24168773, -0.19231297, -0.2168745, -0.24402396, 0.23743293, 0.33595568, 0.260692, 0.19379336, 0.33258808, 0.24039762, 0.23992582, 0.25954893, 0.25570208, 0.32512718, 0.2959324, 0.31510168, 0.28483924, 0.3185611, 0.30045724, 0.29158372, 0.266666, 0.260604, 0.30525047, 0.22269711, 0.3742764, 0.26719987, 0.3802032, 0.28786117, 0.31121463, 0.2750857, 0.3062871, 0.26206675, 0.19328193, 0.28688577, -0.11559406, -0.09537793, 0.032182284, 0.052711282, -0.04492103, -0.027568031, 0.07410127, -0.072352916, -0.007626652, 0.023209194, -0.039342836, -0.029539736, -0.0044076676, -0.01572529, -0.014596638, -0.03823816, 0.017683908, -0.021601297, 0.021558864, 0.035207916, -0.020786418, 0.034444354, 0.07725091, 0.025530286, -0.01761986, 0.008856607, 0.01404422, 0.008442547, -0.0943508, 0.036859952, -0.051700234, -0.009046044, 0.017419044, -0.0023376946, -0.04644509, -0.050842237, -0.07277447, -0.047062766, -0.058517795, -0.060791355, -0.0658082, -0.03332961, -0.085769854, -0.026636243, -0.09536365, -0.050316766, 0.018677544, -0.098109536, -0.021216134, -0.016522456, -0.046691846, -0.045529917, -0.038647927, -0.059120398, -0.008142498, -0.041610643, 0.039049774, -0.037291307, -0.08921782, -0.15739422, 0.13093068, 0.13865064, 0.05042635, 0.13495159, 0.09572868, 0.028025338, 0.117372274, 0.050651185, 0.024095874, 0.036485847, 0.1566715, 0.05535405, 0.035641715, 0.13562398, 0.045102414, 0.06149874, 0.08530422, 0.052710284, 0.07022153, 0.0074974485, 0.02679539, 0.038282692, 0.014634502, 0.028912144, 0.046410885, 0.017663127, 0.034808114, 0.092410125, 0.08629931, 0.019088041, 0.11068031, 0.056276, 0.088959895, 0.05185199, 0.115945674, 0.07802311, 0.09450229, 0.15599267, 0.095298775, 0.073534675, 0.09882146, 0.053236466, 0.13088834, 0.15661591, 0.09986502, 0.11308511, 0.14146113, 0.063594036, 0.1247933, 0.0442555, 0.11039773, 0.064208366, 0.09565961, 0.05985362, 0.0987709, 0.08555728, 0.050416518, 0.12969683, 0.009612192, 0.06339637, 0.04748653, -0.019960033, -0.05487025, -0.017481217, -0.042156156, -0.053125087, 0.052380588, -0.057878584, -0.03146786, -0.06653569, 0.0021610553, -0.048137434, 0.004525723, -0.0056525017, 0.035717193, -0.055798497, -0.007416756, -0.010656371, -0.04619402, 0.03167453, 0.04527099, 0.0003400462, 0.06521659, 0.0142781995, -0.01077174, 0.05131643, -0.07999991, -0.009893199, -0.007068975, 0.004774995, -0.056549583, -0.087971725, -0.027675556, -0.046311136, -0.059103813, -0.06753106, -0.055888582, -0.10263646, -0.0807724, -0.047272097, -0.023929985, -0.08197647, -0.10293827, -0.12000818, -0.1330223, -0.12580197, -0.10385809, -0.07746818, -0.07840628, -0.14482392, -0.037978627, -0.019352581, -0.066911645, -0.07198461, -0.011379985, -0.07264076, -0.02449189, -0.065580316, -0.09051657, -0.069665074, 0.093972586, 0.013487685, -0.0074862274, 0.080473825, 0.021732394, 0.0008204921, 0.11457453, 0.02601418, 0.09633898, 0.08224931, -0.0037200723, 0.048369907, -0.0050847284, 0.0030173468, 0.01835846, 0.09519312, -0.010682773, -0.024561055, 0.038562696, 0.030988228, -0.028099842, 0.028049765, 0.04741106, -0.04944667, 0.014661175, -0.015507648, -0.031054342, 0.010295783, 0.12604347, 0.05721444, 0.14134239, 0.030496782, 0.08401317, 0.091312945, 0.01098424, 0.08826056, 0.1026207, 0.07927635, 0.025364535, 0.05707324, 0.061308738, 0.028861511, 0.1034844, 0.06445801, 0.057386503, 0.055734612, 0.06890258, 0.09716136, 0.047037337, 0.06887161, 0.066361964, 0.04157475, 0.025308384, 0.016055357, 0.06287649, 0.045316193, 0.09893317, 0.075755544, 0.0960373, 0.15379927, -0.035753515, 0.029298266, 0.033884946, -0.04969427, -0.0029530227, 0.0014674123, -0.021002341, -0.035459965, -0.049185745, -0.03931418, 0.028933931, -0.034939144, -0.024996966, -0.047681205, -0.041890357, -0.021956937, -0.06150441, -0.04196604, 0.008135943, -0.032635994, 0.01361963, -0.017533623, -0.05054234, -0.015578086, -0.007467574, 0.0065354584, -0.008506475, 0.043958567, 0.006175616, 0.06612888, -0.18303056, -0.16925016, -0.20937914, -0.119777486, -0.116308294, -0.1280446, -0.12713078, -0.11039728, -0.1610903, -0.1275411, -0.154111, -0.12946108, -0.10113361, -0.11317951, -0.13503867, -0.10671942, -0.2272407, -0.13655199, -0.12720376, -0.123777986, -0.13736893, -0.138655, -0.12353107, -0.16654766, -0.11501093, -0.08404047, -0.09229355, -0.16249195, -0.096868895, -0.23252974, 0.08764129, 0.07616117, 0.11908012, 0.055172153, 0.13713256, 0.0935886, 0.100650646, 0.03615612, 0.061386053, 0.14191131, 0.102920465, 0.10954939, 0.091402635, 0.11547904, 0.1319245, 0.07158662, 0.11833943, 0.074926816, 0.096520774, 0.029011998, 0.10739666, 0.12849662, 0.11942712, 0.07832366, 0.13306901, 0.13991481, 0.016586592, 0.03138015, 0.08957055, 0.0849697, 0.040193997, 0.0043354584, 0.06355289, -0.022522114, -0.07298363, 0.018323138, 0.0042417957, -0.011239402, -0.077842675, -0.028772403, -0.034724835, 0.029979138, -0.058244158, 0.00023837706, -0.03468735, -0.088738844, 0.0043024565, -0.03053603, -0.04955279, -0.024656449, -0.08595048, -0.039450806, -0.06763787, -0.10550461, -0.08389759, -0.09338725, -0.060995284, -0.09993137, -0.13827753, -0.07430939, -0.62138414, -0.5748394, -0.6258963, -0.6168016, -0.55586535, -0.5190644, -0.6255094, -0.60478944, -0.5228399, -0.5604107, -0.6332681, -0.56780535, -0.57544786, -0.6390013, -0.5687759, -0.6073792, -0.604001, -0.63991004, -0.5810448, -0.65262353, -0.5186756, -0.65017825, -0.6577674, -0.51646954, -0.67621225, -0.57765025, -0.55721146, -0.5823297, -0.64085793, -0.4053308, -0.06300295, -0.05799242, -0.04564888, -0.10853826, -0.13153695, -0.030647932, -0.14068064, -0.03994927, -0.043549266, -0.066661835, -0.10260493, -0.13838203, -0.13846649, -0.039528888, -0.13804016, -0.14080955, -0.07171104, -0.14500134, -0.15413988, -0.035968482, -0.12681997, -0.06612908, -0.08888218, -0.1427715, -0.007492848, -0.059319668, -0.009892328, -0.14118707, -0.0963326, -0.16051042, -0.029708382, 0.006496342, 0.020734807, -0.055081163, -0.054122046, -0.038137693, -0.11180523, -0.07773729, -0.10976929, -0.03165385, -0.07089865, -0.012799417, -0.102118246, -0.056088675, -0.030880224, 0.001208622, -0.0029872498, -0.066319644, -0.008374165, 0.0044194665, -0.0622678, -0.03136764, -0.0017007161, -0.060261305, 0.02224322, -0.11784327, -0.118240744, -0.09434458, -0.037106764, -0.023170033, -0.036425848, -0.08523265, -0.06099705, -0.15179452, -0.120483674, -0.044731747, -0.0426257, -0.11566425, -0.1074713, -0.04087459, -0.015691258, -0.11591116, -0.15149657, -0.104848385, -0.09469157, -0.12418427, -0.07002456, -0.112993814, -0.103304274, -0.03987656, -0.13131365, -0.012865097, -0.10810059, -0.14712729, -0.101429, -0.12674725, -0.05282774, -0.06950732, -0.0189551, -0.11815423, -0.016987804, -0.055654164, -0.042242486, -0.039350927, -0.08735059, -0.026930982, 0.011317221, -0.103749186, -0.07292371, -0.013035603, -0.026288163, -0.08674243, -0.007875044, -0.10780092, -0.06372475, -0.09586222, -0.05632068, -0.09309914, -0.0417968, 0.020013709, -0.10513184, -0.062299576, -0.016059635, -0.09427554, -0.11530882, -0.053029034, -0.07987979, -0.057408545, 0.0071102916, -0.091048524, -0.08315952, -0.11259987, -0.12670225, -0.11712536, -0.04811343, -0.033828575, -0.14488848, -0.16759251, -0.15942417, -0.12796707, -0.06646271, -0.12804139, -0.09214782, -0.07333291, -0.014906066, -0.057636686, -0.06755479, -0.029680567, -0.05536946, -0.13066386, -0.01843921, -0.05853724, -0.107828945, -0.15358977, -0.08985749, -0.022014111, -0.12752263, -0.13323829, -0.12335918, -0.14388944};
const float fc1_bias[4] = {0.32392558, -0.007736312, -0.26686862, -0.113932416};
const float fc2_weight[360] = {-0.13197568, -0.19150329, -0.28769574, -0.290487, -0.27929342, -0.21704568, -0.20376235, -0.3620257, -0.2684214, -0.305315, -0.2957417, -0.3028661, -0.26812485, -0.34444734, -0.24547602, -0.24914545, -0.23723061, -0.23272218, -0.24194446, -0.26743153, -0.2571856, -0.25736672, -0.2388266, -0.2550191, -0.1867826, -0.17219289, -0.3175909, -0.33273843, -0.19213799, -0.20881116, 0.0769208, 0.10224788, 0.03343867, 0.048908796, 0.10325775, 0.028934153, -0.021177758, 0.027400326, 0.047422826, 0.09232084, 0.06785564, 0.0163377, 0.033720504, 0.102651976, 0.09367767, 0.037186243, 0.053723775, 0.06559824, 0.03772671, 0.06380624, 0.066334456, 0.09044114, 0.065436296, 0.062875144, 0.11668304, 0.07616675, 0.018391287, 0.028564641, 0.11410427, 0.17376684, -0.054464173, 0.038015656, -0.017708607, 0.0323903, 0.020460375, -0.0030525085, 0.011266909, 0.013404811, 0.0045449208, 0.09405941, 0.059869956, -0.05627339, -0.02015322, 0.03513106, 0.00012994761, 8.8237815e-05, 0.01460533, -0.02270723, 0.05328945, 0.058383286, -0.028518524, 0.01704559, -0.04630977, -0.006592662, 0.058844734, -0.00095937017, -0.06275186, 0.011866221, -0.0041948, -0.017545743, 0.087610304, 0.09356451, 0.119617164, 0.09713676, 0.08422361, 0.005482107, 0.020715453, 0.080487035, 0.027384136, 0.082832135, 0.08206459, 0.0010556736, 0.053920355, 0.04631778, 0.09457328, 0.034042723, 0.039166734, 0.06304502, 0.16387475, 0.0939425, 0.020185119, 0.07667152, 0.022772586, 0.112658426, 0.10971051, 0.09610979, -0.0025437484, 0.005148113, 0.04118222, -0.04776824, -0.094055474, -0.022989074, -0.029047035, 0.024106357, 0.044308987, -0.061812766, 0.013219567, -0.05563612, -0.014971142, -0.041809287, -0.037488747, 0.032422252, -0.013332174, -0.0049981787, -0.034111418, -0.080490276, -0.09223097, -0.040386762, 0.005510649, -0.017283503, -0.049950037, -0.014015659, 0.008904832, -0.009487601, 0.036291733, -0.02527895, 0.010272095, -0.067795485, 0.0133103, -0.017199986, -0.09485422, -0.110533394, -0.0844621, -0.07930336, -0.07183892, -0.058403246, 0.007682937, -0.07253293, -0.12399245, -0.09062245, -0.089352235, -0.12062507, -0.10961872, -0.12097498, -0.13700257, -0.07419866, -0.12479645, -0.16947368, -0.06455004, -0.05100496, -0.034780134, -0.06593915, -0.044100422, -0.10366403, -0.03940199, -0.044652037, -0.08287125, -0.027614556, -0.12248613, -0.11714426, 0.1378904, 0.18772513, 0.26616734, 0.30614808, 0.22257762, 0.26365048, 0.21646336, 0.36590344, 0.3001472, 0.2489602, 0.21882987, 0.22973944, 0.27705014, 0.2854829, 0.2679884, 0.35139498, 0.20941713, 0.2563991, 0.2642196, 0.26162535, 0.22523524, 0.18029901, 0.20433682, 0.22989146, 0.2611553, 0.28868097, 0.24911459, 0.30290174, 0.2103493, 0.2883219, -0.07836543, -0.078688845, -0.059243742, -0.06160636, -0.0008624386, -0.07023338, -0.093398586, -0.04162506, -0.065977186, -0.0032260523, -0.023127677, -0.06930911, -0.070797995, 0.02116326, -0.11072241, -0.065551154, -0.10899293, -0.06765783, -0.12920012, -0.09143192, -0.023642326, -0.04190156, -0.053243473, -0.030168943, 0.00034943904, -0.04446963, -0.01222054, -0.030648518, -0.036121797, -0.12191477, -0.06284151, -0.053105183, -0.0057987347, -0.012114234, 0.028096834, -0.016593361, -0.04742342, -0.06540319, -0.015702372, 0.025156159, -0.021122878, -0.067373715, 0.0089232605, 0.018194983, -0.056717075, 0.009181698, 0.007573827, -0.017105566, 0.04140329, 0.009164489, -0.08450174, 0.007843096, 0.007119603, 0.0018476354, -0.02900991, 0.013531709, -0.024553988, -0.058728438, -0.05439371, -0.08267744, -0.15364474, -0.08295408, -0.050132483, -0.13220042, -0.035944704, -0.11722534, -0.12387044, -0.024415337, -0.12960583, -0.10339187, -0.06031829, -0.045237344, -0.07604152, -0.08241686, -0.110112704, -0.07496489, -0.012034837, -0.08089504, -0.036168408, -0.06876839, -0.0981442, -0.07271286, -0.021460427, -0.0087802345, -0.04088483, -0.12081889, -0.073446505, -0.08581929, 0.04713037, 0.047057137, 0.09888795, 0.006609207, 0.029912358, 0.083346345, 0.015264968, 0.023008093, 0.015696045, 0.030728912, 0.0018111223, -0.026894378, 0.029039135, 0.08403826, 0.04392523, 0.006719153, 0.0046088016, 0.051224366, 0.048452962, 0.020864917, -0.01181686, 0.07248856, 0.06235, -0.03508928, 0.03367385, 0.00033017184, 0.019706003, -0.030858403, -0.0042219465, -0.0049348325, 0.024998426, 0.07518112, 0.060611654, 0.10576032, 0.12199442, 0.031321388, 0.06864193, 0.07046359, 0.079395615, 0.109722696, 0.07352274, 0.09063328, 0.12424692, 0.1508253, 0.11672624, 0.16032535, 0.055434927, 0.09441628, 0.09499493, 0.117639214, 0.12307712, 0.09438266, 0.09411989, 0.02760865, 0.043361403, 0.018224722, 0.052484784, 0.015401312, -0.008885381, 0.109367445, 0.08869208, 0.04777679};
const float fc2_bias[2] = {0.20849772, -0.26646253};