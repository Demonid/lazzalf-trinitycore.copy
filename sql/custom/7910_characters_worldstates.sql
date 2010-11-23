DELETE FROM `worldstates` WHERE `entry` IN (90010,90011);
INSERT INTO `worldstates` (`entry`,`value`,`comment`) VALUES
(90010, 0, 'Wintergrasp: Vehicle count Ally'),
(90011, 0, 'Wintergrasp: Vehicle count Horde');