-- character table
DROP TABLE IF EXISTS `gh_guildadd`;
CREATE TABLE `gh_guildadd` (
  `guildId` int(32) unsigned NOT NULL,
  `GuildHouse_Add` int(32) unsigned NOT NULL,
  PRIMARY KEY  (`guildId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;