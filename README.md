BambooOF
========
this is my jx clone project. all these graphics & audio are extracted from the original game.

GAME ENGINE TASK LIST
---------
* build recast wrapper [OK]
  * develop wrapper [OK]
  * render using of [OK]
* port bamboo from cc to of [WIP]
  * develop 2d sprite renderer/3d plane renderer [OK] [HARD]
  * terrain rendering [OK]
  * text rendering [OK]
  * particle rendering [WIP]
    * 2D [OK]
    * 3D [WIP]
    * optimization
  * render character using sprite renderer [OK]
    * animation loader [OK]
    * state operation [OK]
    * render [OK] [VERY HARD]
    * optimization [OK]
  * re-generate data [OK] [HARD]
  * memory management [OK]
  * memory optimization [OK] [VERY HARD]
* develop rts engine
  * structure render
    * 3d blocks generating algorithm [OK] [HARD]
	* implement [WIP]
	* optimization
  * fog of war calculation
    * base theory [OK]
    * implement [WIP]
	* optimization
  * multi-agent pathfinding
    * base theory [OK]
    * base library [OK]
    * intergrate [OK]
  * dynamic object/obstacle placing
    * base theory [OK]
    * base library [OK]
    * intergrate [OK]
  * combat engine, easiest
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
