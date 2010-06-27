DELETE FROM `creature_model_info` WHERE `modelid` IN (28638, 28821, 28822);
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES
(28638, 0.15, 20, 2, 0),
(28821, 0.35, 20, 2, 0),
(28822, 0.35, 20, 2, 0);