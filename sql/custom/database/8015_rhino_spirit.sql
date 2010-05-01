UPDATE creature_template SET faction_A = 16, faction_H = 16, mindmg = 320, maxdmg = 414, attackpower = 563, dmg_multiplier = 7 WHERE entry = 29791;
DELETE FROM `creature_ai_scripts` WHERE `id` = 2979101;
INSERT INTO `creature_ai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES
('2979101','29791','0','0','100','30','1000','2000','0','0','11','55220','4','0','0','0','0','0','0','0','0','0','Rhino Spirit - Cast Stampede');
