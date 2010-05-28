UPDATE creature_template SET minlevel=83, maxlevel=83, faction_A=14, faction_H=14, unit_flags=0x02000000, flags_extra=0x00000002, ScriptName="npc_flame_tsunami" WHERE entry=30616;

DELETE FROM `gameobject` WHERE `id`=193988;
INSERT INTO `gameobject`
(`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`)
VALUES
(800065, 193988, 615, 3, 1, 3137.26, 501.08, 87.9118, 0.846795, 0, 0, 0.41086,
0.911698, -30, 0, 1),
(67869, 193988, 615, 3, 1, 3238.37, 518.595, 58.9057, 0.739184, 0, 0, 0.361235,
0.932475, -30, 0, 1),
(800032, 193988, 615, 3, 1, 3362.01, 553.726, 95.7068, 4.56818, 0, 0, 0.756211,
-0.654328, -30, 0, 1),
(800170, 193988, 615, 3, 1, 3219.67, 656.795, 87.2898, 5.92596, 0, 0, 0.177664,
-0.984091, 25, 0, 1);

UPDATE creature_template SET ScriptName="npc_twilight_fissure" WHERE entry=30641; 
UPDATE creature_template SET ScriptName="npc_flame_tsunami" WHERE entry=30616;

UPDATE creature_template SET faction_A = 103, faction_H = 103 WHERE entry IN (30641, 31521);