CREATE TABLE `cheat_log` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `cheat_type` varchar(45) NOT NULL,
  `guid` int(10) unsigned NOT NULL,
  `name` varchar(45) NOT NULL,
  `level` int(10) unsigned NOT NULL default 0,
  `map` int(10) unsigned NOT NULL,
  `area` int(10) unsigned NOT NULL,
  `pos_x` float NOT NULL,
  `pos_y` float NOT NULL,
  `pos_z` float NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;