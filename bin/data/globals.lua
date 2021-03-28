-- GLOBALS SCRIPT FILE

-- include the user modifiable options
dofile("data/config.lua")

GAME_TITLE = "Starflight-The Lost Colony"
GAME_VERSION = "2.0"
--STARTUPMODULE = "STARTUP"
--STARTUPMODULE = "SETTINGS"
STARTUPMODULE = "TITLESCREEN"
--STARTUPMODULE = "PLANETORBIT"
--STARTUPMODULE = "STARPORT"
ESCAPEMODULE = "TITLESCREEN"

DEBUG_OUTPUT = true


-- gui components
GUI_MESSAGE_POS_X = 290
GUI_MESSAGE_POS_Y = 544
GUI_MESSAGE_WIDTH = 519
GUI_MESSAGE_HEIGHT = 224
GUI_CONTROLPANEL_POS_X = 809
GUI_CONTROLPANEL_POS_Y = 329
GUI_AUX_POS_X = 0
GUI_AUX_POS_Y = 329
GUI_SOCKET_POS_X = 333
GUI_SOCKET_POS_Y = 491
AUX_SCREEN_X = 12
AUX_SCREEN_Y = 521
AUX_SCREEN_WIDTH = 228
AUX_SCREEN_HEIGHT = 228
GUI_GAUGES_POS_X = 236
GUI_GAUGES_POS_Y = 0
GUI_VIEWER_POS_X = -40
GUI_VIEWER_POS_Y = 30
GUI_VIEWER_SPEED = 12
GUI_VIEWER_LEFT = -440
GUI_VIEWER_RIGHT = -40
GUI_RIGHT_VIEWER_POS_X = 600
GUI_RIGHT_VIEWER_POS_Y = 85


--START components should add up to 3 (incl 1 pod minimum)
--MAX components should add up to 26 (pods = 1/4 point)

-- profession defaults--freelance
PROF_FREELANCE_ENGINE = 1
PROF_FREELANCE_ARMOR = 0
PROF_FREELANCE_SHIELD = 0
PROF_FREELANCE_LASER = 1
PROF_FREELANCE_MISSILE = 0
PROF_FREELANCE_PODS = 1
PROF_FREELANCE_ENGINE_MAX = 5
PROF_FREELANCE_ARMOR_MAX = 4
PROF_FREELANCE_SHIELD_MAX = 4
PROF_FREELANCE_LASER_MAX = 6
PROF_FREELANCE_MISSILE_MAX = 3
PROF_FREELANCE_PODS_MAX = 16 --4 points

-- profession defaults--science
PROF_SCIENCE_ENGINE = 1
PROF_SCIENCE_ARMOR = 1
PROF_SCIENCE_SHIELD = 0
PROF_SCIENCE_LASER = 0
PROF_SCIENCE_MISSILE = 0
PROF_SCIENCE_PODS = 1
PROF_SCIENCE_ENGINE_MAX = 6
PROF_SCIENCE_ARMOR_MAX = 4
PROF_SCIENCE_SHIELD_MAX = 6
PROF_SCIENCE_LASER_MAX = 4
PROF_SCIENCE_MISSILE_MAX = 3
PROF_SCIENCE_PODS_MAX = 12 --3 points

-- profession defaults--military
PROF_MILITARY_ENGINE = 1
PROF_MILITARY_ARMOR = 0
PROF_MILITARY_SHIELD = 0
PROF_MILITARY_LASER = 1
PROF_MILITARY_MISSILE = 0
PROF_MILITARY_PODS = 1
PROF_MILITARY_ENGINE_MAX = 4	
PROF_MILITARY_ARMOR_MAX = 4	
PROF_MILITARY_SHIELD_MAX = 4	
PROF_MILITARY_LASER_MAX = 6
PROF_MILITARY_MISSILE_MAX = 6
PROF_MILITARY_PODS_MAX = 8 --2 points

-- ship component classes
ENGINE1_ACCEL = 0.03
ENGINE2_ACCEL = 0.05
ENGINE3_ACCEL = 0.08
ENGINE4_ACCEL = 0.10
ENGINE5_ACCEL = 0.12
ENGINE6_ACCEL = 0.15
ENGINE7_ACCEL = 0.25
ENGINE1_TOPSPEED = 1.2 -- base
ENGINE2_TOPSPEED = 1.6 -- + .4
ENGINE3_TOPSPEED = 2.2 -- + .6
ENGINE4_TOPSPEED = 2.8 -- + .6
ENGINE5_TOPSPEED = 3.4 -- + .6
ENGINE6_TOPSPEED = 4.2 -- + .8    
ENGINE1_TURNRATE = 1.0
ENGINE2_TURNRATE = 1.4
ENGINE3_TURNRATE = 1.8
ENGINE4_TURNRATE = 2.2
ENGINE5_TURNRATE = 2.6
ENGINE6_TURNRATE = 3.0	

--these should be proportional to weapon damage per second (dps)
--a shield should hold up for several seconds at least (see class 1 weaps)
SHIELD1_STRENGTH = 140 --was 70 (3 seconds of missile-1 hits)
SHIELD2_STRENGTH = 260 --was 100
SHIELD3_STRENGTH = 380 --was 130
SHIELD4_STRENGTH = 500 --was 150
SHIELD5_STRENGTH = 620 --was 225
SHIELD6_STRENGTH = 740 --was 350  (3 seconds of missile-6 hits)

--armor should hold up to several seconds of equivalent class weapon dps (see class 1 weaps)
ARMOR1_STRENGTH = 100 --was 45	then 120 (3 seconds of laser-1 hits)
ARMOR2_STRENGTH = 100 --was 60	then 240
ARMOR3_STRENGTH = 100 --was 80	then 360
ARMOR4_STRENGTH = 100 --was 100	then 480
ARMOR5_STRENGTH = 100 --was 150	then 600
ARMOR6_STRENGTH = 100 --was 250 then 720 (3 seconds of laser-6 hits)

--laser fires at same rate always but higher classes do more damage 
--laser 1 can do 40 pts of dmg per second with direct hits
LASER1_FIRERATE = 70 --was 750 (100 = 1/10 second)
LASER2_FIRERATE = 70 --was 550
LASER3_FIRERATE = 70 --was 350
LASER4_FIRERATE = 70 --was 250
LASER5_FIRERATE = 70 --was 200
LASER6_FIRERATE = 70 --was 150
LASER1_DAMAGE = 4  --was 15 (40 dps)
LASER2_DAMAGE = 8  --was 24 (80 dps)
LASER3_DAMAGE = 12 --was 24 (120 dps)
LASER4_DAMAGE = 16 --was 27 (160 dps)
LASER5_DAMAGE = 20 --was 30 (200 dps)
LASER6_DAMAGE = 48 --was 32 (480 dps)

--missile damage will always be much higher than lasers
--missile 1 can do 80 pts of dmg per second with direct hits
MISSILE1_FIRERATE = 500 --was 750  (500 = 1/2 second)
MISSILE2_FIRERATE = 475 --was 950
MISSILE3_FIRERATE = 450 --was 1150
MISSILE4_FIRERATE = 425 --was 1350
MISSILE5_FIRERATE = 400 --was 1550
MISSILE6_FIRERATE = 375 --was 1750
MISSILE1_DAMAGE = 40  -- (80 dps)
MISSILE2_DAMAGE = 60 
MISSILE3_DAMAGE = 90
MISSILE4_DAMAGE = 130
MISSILE5_DAMAGE = 180
MISSILE6_DAMAGE = 230

--special case items (are these still used?)
LASER7_FIRERATE = 100 
LASER7_DAMAGE = 40
MISSILE7_FIRERATE = 250
MISSILE7_DAMAGE = 250
ENGINE7_TOPSPEED = 5.8
ENGINE7_TURNRATE = 3.0
SHIELD7_STRENGTH = 650


LASER_DURATION = 400
LASER_SPEED    = 20
MISSILE_DURATION = 4000
MISSILE_SPEED    = 10

ALIEN_MISSILE_RANGE = 2000	--was 2000
ALIEN_LASER_RANGE   = 400	--was 400
ALIEN_SAFETY_DISTANCE = 400

--when game starts, this should be next to Myrrdan star system
PLAYER_HYPERSPACE_START_X = 15553
PLAYER_HYPERSPACE_START_Y = 13244

--when player enters star system (should probably be random)
PLAYER_SYSTEM_START_X = 10370
PLAYER_SYSTEM_START_Y = 8350

--when player enters the starport
PLAYER_STARPORT_START_X = 200
PLAYER_STARPORT_START_Y = 430


