BambooOF
========
this is my jx clone project. all these graphics & audio are extracted from the original game.

TASK LIST
---------
* build recast wrapper [OK]
  * develop wrapper [OK]
  * render using of [OK]
* port bamboo from cc to of [DEV]
  * develop 2d sprite renderer/3d plane renderer [OK]
  * terrain rendering [OK]
  * text rendering [DEV]
  * particle rendering [DEV]
  * render character using sprite renderer[DEV]
  * redesign data
  * memory management
* develop rts engine
  * fog of war calculation
  * multi-agent pathfinding
  * dynamic object/obstacle placing
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
