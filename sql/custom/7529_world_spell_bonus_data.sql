-- Mind Flay bonus data
DELETE FROM `spell_bonus_data` WHERE `entry` = 58381;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(58381, 0.257, -1, -1, -1, "Priest - Mind Flay");

-- Seal Fate proc event
UPDATE spell_proc_event SET SpellFamilyMask1 = SpellFamilyMask1 | 4, Cooldown = 1 WHERE entry IN (14195, 14194, 14193, 14190, 14186);

-- Arcane Empowerment proc event
DELETE FROM spell_proc_event WHERE entry IN (31583, 31582, 31579);
INSERT INTO spell_proc_event (entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, procFlags, procEx, ppmRate, CustomChance, Cooldown) VALUES
(31583, 0, 3, 0x20201000, 0x8000, 0x0, 0x00010000, 0x0000002, 0, 0, 0),
(31582, 0, 3, 0x20201000, 0x8000, 0x0, 0x00010000, 0x0000002, 0, 0, 0),
(31579, 0, 3, 0x20201000, 0x8000, 0x0, 0x00010000, 0x0000002, 0, 0, 0);