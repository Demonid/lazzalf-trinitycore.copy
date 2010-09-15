create table `custom_npc_tele_association` (
	`cat_id` double ,
	`dest_id` double 
); 
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','1');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','2');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','3');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','4');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','5');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','6');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','7');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','8');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','9');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','10');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','11');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','12');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','13');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','14');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','15');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','16');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','17');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','18');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','19');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','20');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('1','21');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','22');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','23');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','24');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','25');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','26');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','27');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','28');
insert into `custom_npc_tele_association` (`cat_id`, `dest_id`) values('2','53');

create table `custom_npc_tele_category` (
	`id` double ,
	`name` varchar (2295),
	`flag` tinyint (3),
	`data0` double ,
	`data1` double 
); 
insert into `custom_npc_tele_category` (`id`, `name`, `flag`, `data0`, `data1`) values('1','[Instances Lvl 1-60]','0','0','0');
insert into `custom_npc_tele_category` (`id`, `name`, `flag`, `data0`, `data1`) values('2','[Instances Lvl 70+]','5','70','0');

create table `custom_npc_tele_destination` (
	`id` double ,
	`name` varchar (900),
	`pos_X` float ,
	`pos_Y` float ,
	`pos_Z` float ,
	`map` double ,
	`orientation` float ,
	`level` tinyint (3),
	`cost` double 
); 
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('1','Blackfathom Deeps','4254.58','664.74','-29.04','1','1.97','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('2','Blackrock Depths','-7301.03','-913.19','165.37','0','0.08','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('3','Blackrock Spire','-7535.43','-1212.04','285.45','0','5.29','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('4','Blackwing Lair','-7665.55','-1102.49','400.679','469','0','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('5','Maraudon','-1433.33','2955.34','96.21','1','4.82','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('6','Molten Core','1121.45','-454.317','-101.33','230','3.5','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('7','Naxxramas','3125.18','-3748.02','136.049','0','0','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('8','Onyxia\'s Lair','-4707.44','-3726.82','54.6723','1','3.8','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('9','Razorfen Downs','-4645.08','-2470.85','85.53','1','4.39','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('10','Razorfen Kraul','-4484.04','-1739.4','86.47','1','1.23','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('11','Ruins of Ahn\'Qiraj','-8409.03','1498.83','27.3615','1','2.49757','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('12','Scholomance','1219.01','-2604.66','85.61','0','0.5','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('13','Shadowfang Keep','-254.47','1524.68','76.89','0','1.56','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('14','Stratholme','3263.54','-3379.46','143.59','0','0','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('15','Temple of Ahn\'Qiraj','-8245.84','1983.74','129.072','1','0.936195','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('16','The Deadmines','-11212','1658.58','25.67','0','1.45','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('17','The Scarlet Monastery','2843.89','-693.74','139.32','0','5.11','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('18','The Sunken Temple','-10346.9','-3851.9','-43.41','0','6.09','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('19','The Wailing Caverns','-722.53','-2226.3','16.94','1','2.71','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('20','Zul\'Farrak','-6839.39','-2911.03','8.87','1','0.41','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('21','Zul\'Gurub','-11916.7','-1212.82','92.2868','0','4.6095','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('22','Black Temple','-3610.72','324.988','37.4','530','3.28298','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('23','Coilfang Reservoir','517.288','6976.28','32.0072','530','0','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('24','Gruul\'s Lair','3539.01','5082.36','1.69107','530','0','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('25','Hellfire Citadel','-305.816','3056.4','-2.47318','530','2.01','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('26','Isle Of Quel\'Danas','12947.4','-6893.31','5.68398','530','3.09154','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('27','Karazhan','-11118.8','-2010.84','47.0807','0','0','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('28','Tempest Keep','3089.58','1399.05','187.653','530','4.79407','0','0');
insert into `custom_npc_tele_destination` (`id`, `name`, `pos_X`, `pos_Y`, `pos_Z`, `map`, `orientation`, `level`, `cost`) values('29','Zul\'Aman','6846.95','-7954.5','170.028','530','4.61501','0','0');