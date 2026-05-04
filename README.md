# Comp-3000 - DEPOT DELTA

<img width="900" height="528" alt="image" src="https://github.com/user-attachments/assets/16d5f7af-fa5e-4ce4-8e24-c4d2dfdea015" />


Depot Delta is a Real Time Strategy (RTS) game where you need to manage your resources and personnel to survive incoming waves of zombies. Your supply lines are crucial as this is how you will restock units that are keeping the hordes at bay. With limited survivors and resources to begin with you can adventure to find more but then need to consider how you will return them to the depot so they can help you in the fight. Personal once damaged will need to be rescued by medical units creating a balance of keep fighting or retreat to fight another day as once you lose a unit it is lost for good. 

## Poster
<img width="3178" height="2245" alt="10809998" src="https://github.com/user-attachments/assets/beb55181-6226-4682-b117-c05536e1c124" />


## Inspirations and Motivation
The main inspiration for this game is during my placement year within the MoD I was involved with a variety of logistic based wargames. This made me think about my experience of RTS games such as Call of war, company of heroes or Planetary annihilation titans. In these games you have a central pool of resources you can use anywhere where in the wargames we had to balance combat with ensuring resources were brought to the units and it added another layer for the enemy to target. I feel as though this could add a layer to RTS games that isn’t currently there.

As I also spent time on placement learning about medical supply lines it was tough as it was not always possible to save everyone, and tough choices had to be made. In most RTS games losing a unit means little other than losing ground as you can make a new one of that unit, but I plan for units to be a resource for the player to manage and grow attached to. Forcing them to try and hold the line as long as they can or at least decide when loses are too much to justify the gain.

There are a variety of video games I really enjoy but you must play online against other people. This can be frustrating as it makes entry to these games quite difficult so as a bit resident evil fan I thought a single player mode surviving waves of zombies and other monstrosities would add a good starting point for new players. Allows for learning mechanics as well as creating a unique experience for experienced players.


## Minimal Viable Product (MVP)
The MVP was an endless level where zombies continuously spawn and move towards the depot. The focus of the MVP was the game loop rather than level design. The MVP could be split into different sections being:
Map – the level UI including moving the player’s view
Basic Unit – the units the player had available to them (basic variety was/is the only unit variety in the game)
Basic Zombie – the zombies the player had to defend the depot from
Basic Convoy – the convoys the player could use to restock their units
Base – the depot the player was trying to defend and that all their resources came from
Tech Tree – the way the player made upgrades to their units, convoys & resource production

## Minimal Awesome Product (MAP)
The MAP further developed the MVP taking the prototype. Updating existing artwork and improving systems such as adding new zombie variants. The main difference between MVP and MAP is the MAP includes levels with goals to complete as well as a tutorial. A new mechanic was added being buildings on the map. Buildings generate resources faster than the depot specialising in specific resources. Finally, music and sound effects were added to help guide the player.


## Design
Map:
<img width="456" height="455" alt="image" src="https://github.com/user-attachments/assets/162383ca-4904-4bb8-9f09-c56a855194ce" />


Basic Unit:
<img width="250" height="250" alt="image" src="https://github.com/user-attachments/assets/8f9b09c6-4089-4719-a525-3e9eec6bc315" />


Basic Zombie:
<img width="269" height="252" alt="image" src="https://github.com/user-attachments/assets/fcb64eb6-1439-40ae-af51-fadc90e50b4a" />

Brute Zombie:
<img width="304" height="304" alt="image" src="https://github.com/user-attachments/assets/f77d20ef-d9e0-422d-bff7-4f1fc17d8c86" />

Quick Zombie:
<img width="254" height="254" alt="image" src="https://github.com/user-attachments/assets/e6bb0b45-b8cd-41f1-a5b6-c58875e2a7fd" />


Basic Convoy:
<img width="277" height="277" alt="image" src="https://github.com/user-attachments/assets/de174770-e5d8-4829-93b3-3f8082b7335c" />


Base:
<img width="422" height="254" alt="image" src="https://github.com/user-attachments/assets/906f7147-3892-45fd-a4e6-8370d706ddac" />


Tech tree:
<img width="1272" height="717" alt="Screenshot 2026-05-04 204758" src="https://github.com/user-attachments/assets/8e127f3e-869f-4984-bc22-b4e40a134175" />


## Tool and Technologies
### Project management tools
Trello was used as a kanban board. Trello allowed for storing a variety of information such as the product backlog, list of MVP features and project motivation. The board was separated into lists with cards in each list. Each sprint had its own list and separate lists for during the sprints being “Tasks for today”, “Tasks done today”, “Sprint ‘X’ done”. Colour coding allowed for leaving notes in “Tasks for today” for the following day(s). The “Consider down the line” colour is used for work to be completed post-project.
Version control.

Git was used for version control through GitHub. This was done through the command line for more direct control. There was minimal use of the GitHub web app and desktop app for help solving tricky issues that occurred. Branching using GitHub allows for working on new features without affecting the latest working build. This was not used during DD development so far. Further work on DD will be done in separate branching now the demo is complete. Allowing for the potential of releasing it in early access while still developing. 
Progress tracking.


### Development tools & technologies

For development of DD SDL3 was chosen due to the control it gives developers. As C++ was being used for SDL3 this helped improved the game optimisation. Not using an engine meant common features (such as buttons) had to be developed from scratch. Creating a chance to learn the importance of structure in development using OOP to make reusable features. SDL_ttf was used to handle text and SDL_image was used for graphics rendering. Irrklang allowed for audio handling in the game but added the previously mention restrictions due to its license. Development was done in Visual studio community 2022.
Used to develop Depot Delta artwork.

Google Gemini was used to develop the artwork due to how time-consuming art can be. This focus of this project is on the code so AI placeholder art allowed for more time to be allocated elsewhere. Gemini adds it’s watermark and backgrounds to the art it generates so Krita was used to remove these and make appropriate changes. Krita is completely free to use unlike Aesprite which was used to created the initial pixelart design. 

XML files are used to store the levels for DD which are handled by using TinyXML2 [11]. XML was used due to its ability to clearly store large quantities of data compared to other options such as JSON. XML files are used for each level as well as the tech tree. A current level xml files stores the current progress of the player in a level.
For designing the levels when using pixelart, Ogmo editor was used for the placing on objects and map design. This allowed for designing tile map levels placing each object manually. Pivoting to the new art theme made Ogmo obsolete due to no longer using a tile map. A new way to place zombies into the level was required without typing the position for every zombie in the XML file (potential of hundreds of zombies per level). A software which spawned a given number of zombies of given types was created. 


## Object Orientated Design
Objects in Depot Delta refer to the different type of units, convoys, buildings or enemies found on the map

### Object class diagrams
<img width="975" height="666" alt="image" src="https://github.com/user-attachments/assets/7fa9da84-1ad5-49ec-8103-f585a13ea423" />

<img width="975" height="882" alt="image" src="https://github.com/user-attachments/assets/a87a5d35-94fc-4555-a1ee-65d51f9650c1" />

### Component Game programming pattern
Components inherit from the component class and are used to create reusable selectable code snippets for the different objects in the game. There are currently 7 different components used in DD:
- Attack component
  Works out the closest zombie to attack
  Checks the attack delay for if the unit can attack
-	Button component
  Checks for clicking on a unit
-	Nearest component
  Used by zombies to find the nearest target for them
-	Pathfinding component
  Calculates the objects flow field
-	Render component
  Renders the object during runtime
-	Resource component
  Stores the objects resources
  Handles adjusting resources when appropriate
-	Resource transfer component
  Checks if transfers can/can’t be done
  Initiates the transfers 
  Handles current transfers

<img width="975" height="739" alt="image" src="https://github.com/user-attachments/assets/5a5eb280-b4a3-4d2d-b867-998c1d10be49" />

### State Game programming pattern
There are a variety of different states used for handling user input. All of which expand the SelectedState class and are used by the HandleSelected class.

<img width="618" height="384" alt="image" src="https://github.com/user-attachments/assets/f437d9ae-1b22-4faa-9dc0-d42a70b1c2ed" />

<img width="975" height="423" alt="image" src="https://github.com/user-attachments/assets/f3b53af4-e842-4e2c-a016-1726f20f1a4e" />

