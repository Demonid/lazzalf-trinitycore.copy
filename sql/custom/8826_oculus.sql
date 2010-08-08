-- Fix ad oculus
UPDATE creature_template SET mindmg = 1000, maxdmg = 1200, spell1 = 39824 WHERE entry = 27653; -- Phantasmal Water
UPDATE creature_template SET mindmg = 1100, maxdmg = 1300, spell1 = 20545 WHERE entry = 27650; -- Phantasmal Air
UPDATE creature_template SET mindmg = 1300, maxdmg = 1500, spell1 = 50744 WHERE entry = 27651; -- Phantasmal Fire
UPDATE creature_template SET mindmg = 1300, maxdmg = 1500, spell1 = 50730, spell2 = 50731 WHERE entry = 27647; -- Phantasmal Ogre
UPDATE creature_template SET mindmg = 1300, maxdmg = 2000, spell1 = 49711, spell2 = 50732 WHERE entry = 27648; -- Phantasmal Naga
UPDATE creature_template SET mindmg = 1300, maxdmg = 2000, dmg_multiplier = 1 WHERE entry = 27649; -- Phantasmal Murloc
UPDATE creature_template SET mindmg = 2000, maxdmg = 2500, spell1 = 32323, spell2 = 51253 WHERE entry = 27642; -- Phantasmal Mammouth
UPDATE creature_template SET mindmg = 2000, maxdmg = 2500, spell1 = 50729, spell2 = 50728 WHERE entry = 27644; -- Phantasmal Worg
UPDATE creature_template SET mindmg = 2000, maxdmg = 2500, spell1 = 59223, spell2 = 59217 WHERE entry = 27645; -- Phantasmal Cloudscraper
UPDATE creature_template SET npcflag = 1 WHERE entry IN (27657, 27658, 27659);
