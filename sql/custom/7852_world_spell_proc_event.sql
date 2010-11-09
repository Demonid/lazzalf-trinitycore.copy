UPDATE spell_proc_event SET procFlags = 139944 WHERE entry IN (974, 32593, 32594, 49283, 49284);
DELETE FROM spell_bonus_data WHERE entry=974;
INSERT INTO `spell_bonus_data` (`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
(   974, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x000222A8, 0x00000000,   0,   0,   3); -- Earth Shield (Rank 1)
DELETE FROM spell_bonus_data WHERE entry=32593;
INSERT INTO `spell_bonus_data` (`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
( 32593, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x000222A8, 0x00000000,   0,   0,   3); -- Earth Shield (Rank 2)
DELETE FROM spell_bonus_data WHERE entry=32594;
INSERT INTO `spell_bonus_data` (`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
( 32594, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x000222A8, 0x00000000,   0,   0,   3); -- Earth Shield (Rank 3)
DELETE FROM spell_bonus_data WHERE entry=49283;
INSERT INTO `spell_bonus_data` (`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
( 49283, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x000222A8, 0x00000000,   0,   0,   3); -- Earth Shield (Rank 4)
DELETE FROM spell_bonus_data WHERE entry=49284;
INSERT INTO `spell_bonus_data` (`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
( 49284, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x000222A8, 0x00000000,   0,   0,   3); -- Earth Shield (Rank 5)
