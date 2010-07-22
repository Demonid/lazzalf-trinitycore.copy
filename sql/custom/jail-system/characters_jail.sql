/*
MySQL Data Transfer
Source Host: localhost
Source Database: 01
Target Host: localhost
Target Database: 01
Date: 12.11.2008 00:55:44
*/
DROP TABLE IF EXISTS `jail_conf`;
SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for jail_conf
-- ----------------------------
CREATE TABLE `jail_conf` (
  `id` int(11) NOT NULL auto_increment,
  `obt` varchar(50) default NULL,
  `jail_conf` int(11) default NULL,
  `jail_tele` float default NULL,
  `help_ger` varchar(255) character set latin1 default '',
  `help_enq` varchar(255) default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `jail_conf` VALUES ('1', 'm_jailconf_max_jails', '3', null, 'Numero di volte che il PG viene impriginato di avere una punizione \r\nDefault = 3\r\n', '');
INSERT INTO `jail_conf` VALUES ('2', 'm_jailconf_max_duration', '672', null, 'Massimo tempo di permamenza in prigione \r\nDefault = 672\r\n', '');
INSERT INTO `jail_conf` VALUES ('3', 'm_jailconf_min_reason', '25', null, 'Lunghezza minima della ragione della prigionia \r\nDefault = 25 \r\n\r\n', '');
INSERT INTO `jail_conf` VALUES ('4', 'm_jailconf_warn_player', '1', null, 'Avvertire il player che sta per subire la punizione se verrà imprigionato un altra volta \r\nAttivato  = 1 Disattivato = 0\r\n', '');
INSERT INTO `jail_conf` VALUES ('5', 'm_jailconf_amnestie', '180', null, 'Ogni quanti giorni i dati della prigione per il personaggio vengono azzerati \r\nDefault = 180 \r\n  Disattivati = 0\r\n', '');
INSERT INTO `jail_conf` VALUES ('6', 'm_jailconf_ally_x', null, '31.7282', 'Teleport Alliance  X \r\nStandart = 31,7282\r\n', '');
INSERT INTO `jail_conf` VALUES ('7', 'm_jailconf_ally_y', null, '135.794', 'Teleport Alliance  Y \r\nStandart = 135,794\r\n', '');
INSERT INTO `jail_conf` VALUES ('8', 'm_jailconf_ally_z', null, '-40.0508', 'Teleport Alliance  Z \r\nStandart = -40,0508', '');
INSERT INTO `jail_conf` VALUES ('9', 'm_jailconf_ally_o', null, '4.73516', 'Teleport Alliance O \r\nStandart = 4,73516', '');
INSERT INTO `jail_conf` VALUES ('10', 'm_jailconf_ally_m', '35', null, 'Teleport Alliance  Map\r\nStandart = 35', '');
INSERT INTO `jail_conf` VALUES ('11', 'm_jailconf_horde_x', null, '2179.85', 'Teleport Horde  X\r\nStandart = \r\n', '');
INSERT INTO `jail_conf` VALUES ('12', 'm_jailconf_horde_y', null, '-4763.96', 'Teleport Horde  Y\r\nStandart = -4763,96', '');
INSERT INTO `jail_conf` VALUES ('13', 'm_jailconf_horde_z', null, '54.911', 'Teleport Horde  Z\r\nStandart = 54,911', '');
INSERT INTO `jail_conf` VALUES ('14', 'm_jailconf_horde_o', null, '4.44216', 'Teleport  Horde O\r\nStandart = 4,44216', '');
INSERT INTO `jail_conf` VALUES ('15', 'm_jailconf_horde_m', '1', null, 'Teleport Horde  Map\r\nStandart = 1', '');
INSERT INTO `jail_conf` VALUES ('16', 'm_jailconf_ban', '0', null, 'Tipo della punizione 1 = account bannato, 2 = pg delettato, 0 =  disattivato', '');
INSERT INTO `jail_conf` VALUES ('17', 'm_jailconf_radius', '10', null, 'Massima distanza di allontanamento dal punto della prigione \r\nDefault = 10\r\n', '');

/*
MySQL Data Transfer
Source Host: localhost
Source Database: 01
Target Host: localhost
Target Database: 01
Date: 12.11.2008 00:08:28
*/
DROP TABLE IF EXISTS `jail`;
SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for jail
-- ----------------------------
CREATE TABLE `jail` (
  `guid` int(11) unsigned NOT NULL COMMENT 'GUID of the jail brother',
  `char` varchar(13) NOT NULL COMMENT 'Jailed charname',
  `release` int(11) unsigned NOT NULL COMMENT 'Release time for the char',
  `amnestietime` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL COMMENT 'Reason for the jail',
  `times` int(11) unsigned NOT NULL COMMENT 'How many times this char already was jailed',
  `gmacc` int(11) unsigned NOT NULL COMMENT 'Used GM acc to jail this char',
  `gmchar` varchar(13) NOT NULL COMMENT 'Used GM char to jail this char',
  `lasttime` timestamp NOT NULL default '0000-00-00 00:00:00' on update CURRENT_TIMESTAMP COMMENT 'Last time jailed',
  `duration` int(11) unsigned NOT NULL default '0' COMMENT 'Duration of the jail',
  PRIMARY KEY  (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Jail table for MaNGOS by WarHead';

-- ----------------------------
-- Records 
-- ----------------------------
