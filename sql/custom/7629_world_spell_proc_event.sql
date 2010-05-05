-- Rune strike aura remove
DELETE FROM spell_proc_event WHERE entry = 56817;
INSERT INTO spell_proc_event (entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, procFlags, procEx, ppmRate, CustomChance, Cooldown) VALUES
(56817, 0, 15, 0, 0x20000000, 0, 0, 0, 0, 0, 0);
