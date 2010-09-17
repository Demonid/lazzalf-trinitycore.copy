DELETE FROM `creature` WHERE `id`=33186;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(33186, 603, 1, 1, 0, 0, 587.073, -203.205, 441.237, 1.60532, 604800, 0, 0, 3555975, 0, 0, 0);

UPDATE `creature_template` SET `unit_flags` = 33554438 WHERE `entry` = 33184;