-- Surge of power (NPC)
DELETE FROM creature WHERE id = 30334;
DELETE FROM creature_addon WHERE guid = 132303;
-- Spawn Focusing iris per summonare malygos
DELETE FROM gameobject WHERE id IN (193958, 193960);
INSERT INTO gameobject VALUES
(NULL, 193958, 616, 1, 1, 754.362, 1301.61, 266.171, 6.23742, 0, 0, 0.022883, -0.999738, 300, 0, 1), 
(NULL, 193960, 616, 2, 1, 754.362, 1301.61, 266.171, 6.23742, 0, 0, 0.022883, -0.999738, 300, 0, 1); 
-- Fix alla chest dei drop
UPDATE gameobject_template SET faction = 35, flags = 0 WHERE entry IN (193967, 193905);
-- Fix loot for Malygos (Alexstrasza's Gift)
DELETE FROM reference_loot_template WHERE entry = 50008;
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
(50008, 40474, 0, 1, 1, 1, 1, 0, 0, 0), -- Surge Needle Ring
(50008, 40497, 0, 1, 1, 1, 1, 0, 0, 0), -- Black Ice
(50008, 40489, 0, 1, 1, 1, 1, 0, 0, 0), -- Greatstaff of the Nexus
(50008, 40526, 0, 1, 1, 1, 1, 0, 0, 0), -- Gown of the Spell-Weaver
(50008, 40511, 0, 1, 1, 1, 1, 0, 0, 0), -- Focusing Energy Epaulets
(50008, 40475, 0, 1, 1, 1, 1, 0, 0, 0), -- Barricade of Eternity
(50008, 40488, 0, 1, 1, 1, 1, 0, 0, 0), -- Ice Spire Scepter
(50008, 40491, 0, 1, 1, 1, 1, 0, 0, 0), -- Hailstorm
(50008, 40519, 0, 1, 1, 1, 1, 0, 0, 0), -- Footsteps of Malygos
(50008, 40486, 0, 1, 1, 1, 1, 0, 0, 0); -- Necklace of the Glittering Chamber
-- Fix loot 25 Man
DELETE FROM reference_loot_template WHERE entry = 50009;
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
(50009, 40592, 0, 1, 1, 1, 1, 0, 0, 0), -- Boots of Healing Energies
(50009, 40566, 0, 1, 1, 1, 1, 0, 0, 0), -- Unravelling Strands of Sanity
(50009, 40194, 0, 1, 1, 1, 1, 0, 0, 0), -- Blanketing Robes of Snow
(50009, 40543, 0, 1, 1, 1, 1, 0, 0, 0), -- Blue Aspect Helm
(50009, 40590, 0, 1, 1, 1, 1, 0, 0, 0), -- Elevated Lair Pauldrons
(50009, 40560, 0, 1, 1, 1, 1, 0, 0, 0), -- Leggings of the Wanton Spellcaster
(50009, 40589, 0, 1, 1, 1, 1, 0, 0, 0), -- Legplates of Sovereignty
(50009, 40555, 0, 1, 1, 1, 1, 0, 0, 0), -- Mantle of Dissemination
(50009, 40591, 0, 1, 1, 1, 1, 0, 0, 0), -- Melancholy Sabatons
(50009, 40594, 0, 1, 1, 1, 1, 0, 0, 0), -- Spaulders of Catatonia
(50009, 40588, 0, 1, 1, 1, 1, 0, 0, 0), -- Tunic of the Artifact Guardian
(50009, 40549, 0, 1, 1, 1, 1, 0, 0, 0), -- Boots of the Renewed Flight
(50009, 40539, 0, 1, 1, 1, 1, 0, 0, 0), -- Chestguard of the Recluse
(50009, 40541, 0, 1, 1, 1, 1, 0, 0, 0), -- Frosted Adroit Handguards
(50009, 40562, 0, 1, 1, 1, 1, 0, 0, 0), -- Hood of Rationality
(50009, 40561, 0, 1, 1, 1, 1, 0, 0, 0), -- Leash of Heedless Magic
(50009, 40532, 0, 1, 1, 1, 1, 0, 0, 0), -- Living Ice Crystals
(50009, 40531, 0, 1, 1, 1, 1, 0, 0, 0), -- Mark of Norgannon
(50009, 40564, 0, 1, 1, 1, 1, 0, 0, 0), -- Winter Spectacle Gloves
(50009, 40558, 0, 1, 1, 1, 1, 0, 0, 0); -- Arcanic Tramplers
-- Emblemi,quest item,Mount
DELETE FROM gameobject_loot_template WHERE entry IN (26094, 26097);
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
(26094, 40752, 100, 1, 0, 2, 2, 0, 0, 0), -- Emblem of Heroism x2
(26094, 1, 100, 1, 0, -50008, 2, 0, 0, 0), -- 2 items level 213
(26094, 44650, 100, 1, 0, 1, 1, 9,  13384, 0), -- Quest item, Judgement at the Eye of Eternity
(26094, 43953, 1, 1, 0, 1, 1, 0, 0, 0), -- Reins of the Blue Drake 	
(26097, 40753, 100, 1, 0, 2, 2, 0, 0, 0), -- Emblem of Valor x2
(26097, 1, 100, 1, 0, -50009, 4, 0, 0, 0), -- 4 items ilevel 226
(26097, 44651, 100, 1, 0, 1, 1, 9, 13385, 0), -- Quest item, Heroic Judgement at the Eye of Eternity
(26097, 43952, 1, 1, 0, 1, 1, 0, 0, 0); -- Reins of the Azure Drake

-- Fix Text 
DELETE FROM script_texts WHERE entry BETWEEN -1616034 AND -1616000;
INSERT INTO script_texts (npc_entry, entry, content_default, sound, type, language, emote, comment) VALUES
(28859, -1616000, 'Lesser beings, intruding here! A shame that your excess courage does not compensate for your stupidity!', 14512, 1, 0, 0, 'Malygos INTRO 1'),
(28859, -1616001, 'None but the blue dragonflight are welcome here! Perhaps this is the work of Alexstrasza? Well then, she has sent you to your deaths.', 14513, 1, 0, 0, 'Malygos INTRO 2'),
(28859, -1616002, 'What could you hope to accomplish, to storm brazenly into my domain? To employ MAGIC? Against ME?', 14514, 1, 0, 0, 'Malygos INTRO 3'),
(28859, -1616003, 'I am without limits here... the rules of your cherished reality do not apply... In this realm, I am in control...', 14515, 1, 0, 0, 'Malygos INTRO 4'),
(28859, -1616004, 'I give you one chance. Pledge fealty to me, and perhaps I won\'t slaughter you for your insolence!', 14516, 1, 0, 0, 'Malygos INTRO 5'),
(28859, -1616005, 'My patience has reached its limit, I WILL BE RID OF YOU!', 14517, 1, 0, 0, 'Malygos AGGRO 1'),
(28859, -1616006, 'Watch helplessly as your hopes are swept away...', 14525, 1, 0, 0, 'Malygos VORTEX'),
(28859, -1616007, 'I AM UNSTOPPABLE!', 14533, 1, 0, 0, 'Malygos SPARK BUFF'),
(28859, -1616008, 'Your stupidity has finally caught up to you!', 14519, 1, 0, 0, 'Malygos SLAY 1-1'),
(28859, -1616009, 'More artifacts to confiscate...', 14520, 1, 0, 0, 'Malygos SLAY 1-2'),
(28859, -1616010, 'How very... naive...', 14521, 1, 0, 0, 'Malygos SLAY 1-3'),
(28859, -1616012, 'I had hoped to end your lives quickly, but you have proven more...resilient then I had anticipated. Nonetheless, your efforts are in vain, it is you reckless, careless mortals who are to blame for this war! I do what I must...And if it means your...extinction...THEN SO BE IT!', 14522, 1, 0, 0, 'Malygos PHASEEND 1'),
(28859, -1616013, 'Few have experienced the pain I will now inflict upon you!', 14523, 1, 0, 0, 'Malygos AGGRO 2'),
(28859, -1616014, 'YOU WILL NOT SUCCEED WHILE I DRAW BREATH!', 14518, 1, 0, 0, 'Malygos DEEP BREATH'),
(28859, -1616016, 'I will teach you IGNORANT children just how little you know of magic...', 14524, 1, 0, 0, 'Malygos ARCANE OVERLOAD'),
(28859, -1616020, 'Your energy will be put to good use!', 14526, 1, 0, 0, 'Malygos SLAY 2-1'),
(28859, -1616021, 'I AM THE SPELL-WEAVER! My power is INFINITE!', 14527, 1, 0, 0, 'Malygos SLAY 2-2'),
(28859, -1616022, 'Your spirit will linger here forever!', 14528, 1, 0, 0, 'Malygos SLAY 2-3'),
(28859, -1616017, 'ENOUGH! If you intend to reclaim Azeroth\'s magic, then you shall have it...', 14529, 1, 0, 0, 'Malygos PHASEEND 2'),
(28859, -1616018, 'Now your benefactors make their appearance...But they are too late. The powers contained here are sufficient to destroy the world ten times over! What do you think they will do to you?', 14530, 1, 0, 0, 'Malygos PHASE 3 INTRO'),
(28859, -1616019, 'SUBMIT!', 14531, 1, 0, 0, 'Malygos AGGRO 3'),
(28859, -1616026, 'The powers at work here exceed anything you could possibly imagine!', 14532, 1, 0, 0, 'Malygos STATIC FIELD'),
(28859, -1616023, 'Alexstrasza! Another of your brood falls!', 14534, 1, 0, 0, 'Malygos SLAY 3-1'),
(28859, -1616024, 'Little more then gnats!', 14535, 1, 0, 0, 'Malygos SLAY 3-2'),
(28859, -1616025, 'Your red allies will share your fate...', 14536, 1, 0, 0, 'Malygos SLAY 3-3'),
(28859, -1616027, 'Still standing? Not for long...', 14537, 1, 0, 0, 'Malygos SPELL 1'),
(28859, -1616028, 'Your cause is lost!', 14538, 1, 0, 0, 'Malygos SPELL 1'),
(28859, -1616029, 'Your fragile mind will be shattered!', 14539, 1, 0, 0, 'Malygos SPELL 1'),
(28859, -1616030, 'UNTHINKABLE! The mortals will destroy... e-everything... my sister... what have you-', 14540, 1, 0, 0, 'Malygos DEATH'),
(32295, -1616031, 'I did what I had to, brother. You gave me no alternative.', 14406, 1, 0, 0, 'Alexstrasza OUTRO 1'),
(32295, -1616032, 'And so ends the Nexus War.', 14407, 1, 0, 0, 'Alexstrasza OUTRO 2'),
(32295, -1616033, 'This resolution pains me deeply, but the destruction, the monumental loss of life had to end. Regardless of Malygos\' recent transgressions, I will mourn his loss. He was once a guardian, a protector. This day, one of the world\'s mightiest has fallen.', 14408, 1, 0, 0, 'Alexstrasza OUTRO 3'),
(32295, -1616034, 'The red dragonflight will take on the burden of mending the devastation wrought on Azeroth. Return home to your people and rest. Tomorrow will bring you new challenges, and you must be ready to face them. Life...goes on.', 14409, 1, 0, 0, 'Alexstrasza OUTRO 4');

-- Aggro Malygos
UPDATE creature_model_info SET combat_reach = '30' WHERE modelid = 26752;
-- Power spark verso Malygos
DELETE FROM spell_script_target WHERE entry = 56152 AND targetEntry = 28859;
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES 
('56152', '1', '28859');
-- Script GO Focusing Iris
UPDATE `gameobject_template` SET `ScriptName` = 'go_malygos_iris' WHERE `entry` IN (193960,193958); 
-- Gob
UPDATE gameobject_template set data17 = 0 where entry = 193908; -- Exit portal
