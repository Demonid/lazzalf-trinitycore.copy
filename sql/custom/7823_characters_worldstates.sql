DELETE FROM `worldstates` WHERE `entry` IN (90001,90002,90003,90004,90005,90006,90007,90008,90009);
INSERT INTO `worldstates` (`entry`,`value`,`comment`) VALUES
(90001, 0, 'Wintergrasp: Current team id'),
(90002, 0, 'Wintergrasp: Is currently wartime?'),
(90003, 0, 'Wintergrasp: Timer'),
(90004, 0, 'Wintergrasp: Clock Ally'),
(90005, 0, 'Wintergrasp: Clock Horde'),
(90006, 0, 'Wintergrasp: Workshop count Ally'),
(90007, 0, 'Wintergrasp: Workshop count Horde'),
(90008, 0, 'Wintergrasp: Tower destroyed Ally'),
(90009, 0, 'Wintergrasp: Tower destroyed Horde');
