DROP TABLE IF EXISTS `guildhouses`;
CREATE TABLE `guildhouses` (
  `id` int(16) unsigned NOT NULL auto_increment,
  `guildId` bigint(20) NOT NULL default '0',
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `map` int(16) NOT NULL,
  `comment` varchar(255) NOT NULL default '',
  `price` bigint(20) NOT NULL default '0',
  `faction` int(8) unsigned NOT NULL default '3',
  `minguildsize` int(16) unsigned NOT NULL default '1',
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;

DROP TABLE IF EXISTS `guildhouses_add`;
CREATE TABLE `guildhouses_add` (
  `guid` int(32) unsigned NOT NULL,  
  `type` int(16) unsigned NOT NULL,
  `id` int(16) unsigned NOT NULL,
  `add_type` int(16) unsigned NOT NULL,
  `comment` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`guid`,`type`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;

DROP TABLE IF EXISTS `guildhouses_addtype`;
CREATE TABLE `guildhouses_addtype` (
  `add_type` int(16) unsigned NOT NULL,
  `comment` varchar(255) NOT NULL default '',
  `price` bigint(20) NOT NULL default '1000',
  `minguildsize` int(16) unsigned NOT NULL default '1',
  PRIMARY KEY  (`add_type`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;

/* -- character table
DROP TABLE IF EXISTS `gh_guildadd`;
CREATE TABLE `gh_guildadd` (
  `guildId` int(32) unsigned NOT NULL,
  `GuildHouse_Add` int(32) unsigned NOT NULL,
  PRIMARY KEY  (`guildId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;*/
/*
DELETE FROM `creature_template` WHERE `entry` = 70058;
INSERT INTO `creature_template` VALUES
('70058','0','0','0','0','0','16042','0','0','0','Guardia','Guild Housing', NULL,'0','80','80','1','14','14','4224','1.71','2','1','1','252','357','0','304','4.6','2000','0','1','33024','0','0','0','0','0','0','215','320','44','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','1','3','1','1','1','0','0','0','0','0','0','0','0','1','310','0','2','guild_guard','0');
*/
/*
INSERT INTO `guildhouses` (`id`, `guildId`, `x`, `y`, `z`, `map`, `comment`) VALUES
('2','0','-10711','2483','8','1','Tauren village at Veiled Sea (Silithus)'),
('4','0','7368','-1560','163','1','Troll Village in mountains (Darkshore)'),
('8','0','-723','-1076','179','1','Tauren camp (Mulgore, Red Rock)'),
('9','0','-206','1666','80','0','Shadowfang Keep an outside instance (Silverpine Forest)'),
('12','0','-4844','-1066','502','0','Old Ironforge'),
('16','1','4303','-2760','16.8','0','Quel\'Thalas Tower'),
('20','0','-9296','670','132','0','Villains camp outside an Stormwind (Elwynn Forest)');
*/