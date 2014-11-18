BambooOF
========
this is my jx clone project. all these graphics & audio are extracted from the original game.

GAME ENGINE TASK LIST
---------
* build recast wrapper [OK]
  * develop wrapper [OK]
  * render using of [OK]
* port bamboo from cc to of [WIP]
  * develop 2d sprite renderer/3d plane renderer [OK]
  * terrain rendering [OK]
  * text rendering [OK]
  * particle rendering [WIP]
    * 2D [OK]
    * 3D [WIP]
    * optimization
  * render character using sprite renderer [WIP]
    * animation loader [OK]
    * state operation [OK]
    * render [WIP]
    * optimization
  * redesign data [OK]
  * memory management [OK]
* develop rts engine
  * fog of war calculation
    * base theory [OK]
    * base library [WIP]
    * intergrate
  * multi-agent pathfinding
    * base theory [WIP]
    * base library [OK]
  * dynamic object/obstacle placing
    * base theory [OK]
    * base library [OK]
  * combat engine
    * Actor
    * Fighter:Actor
    * Mob:Actor
    * Missile:Actor
    * Dummy:Actor
    * Damage
    * Ability
    * Skill: Ability
    * ....

SKILL ARCHITECTURE
---------
- book
	- fight skill
		- fight ability
			- buff
			- damage
			- target
			- script(lua)
		- fight ability cache
		- fight skill runtime data
	- fight skill cache
	- trigger skill
	- trigger skill cache
	- passive skill
	- passive skill cache
- book cache
- book set
