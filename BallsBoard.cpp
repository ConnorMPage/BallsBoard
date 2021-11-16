// BallsBoard.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

enum ESphereMovement { Left, Right, Up, Down, Wait };//creates the ESphereMovement enum, this will include five states for the left,up,right,down movement dorections and the wait for the stationary spheres
enum ESphere1Texture {Violet, Blue, Indigo, Green};//creates the ESphere1Texture enum, each state is the colour of sphere 1 and when that is changed the other thre sphere colours rotate in a clockwise direction
enum EVerticalMovement {VUP, VDown, Stationary};//creates the EVerticalMovemnt enum, there is two states that define the spheres vertical movemnt direction and one to show that the balls are currently stationary
void main()
{
	// the next five variables are used to define the speed of the spheres and speed change
	const float KGameSpeed = 0.1f;// the constant speed
	float speedChange = 1;// this is the change in speed, it starts at 1 but can be changed with the mouse wheel
	float sphereSpeed = KGameSpeed * speedChange;//the sphereSpeed is calculated by multiplying KGameSpeed by speedChange
	const float sphereYSpeed= 0.01f;//sphereYspeed is the speed of which the balls move vertically
	float wheelChange;// can either be 1 or -1, this is dependant on the direction that the mouse wheel is scrolled
	const float maximumSpeed = 5;//max amount that KGameSpeed can be multiplied by
	const float minimumSpeed = 0.5;//the minimum that KGameSpeed can e multiplied by

	//these are the two boolean variables 
	bool isPaused = false;//starts as false as the game begins in play
	bool touchingFloor = true;// used to decide if the balls will roll or not

	//
	const float sphereMovementUpBoundary = 75;//this is the Up boundary for all sphere tracks
	const float sphereMovementDownBoundary = -125;//this is the Down boundary for all of the sphere tracks
	//the left and right boundaries for the sphere1 track, this affects sphere1 and sphere4
	const float sphere1MovementTrackLeftBoundary = -75;
	const float sphere1MovementTrackRightBoundary = 125;
	//the left and right boundary for the sphere2 track this affects sphere2 and sphere3
	const float sphere2MovementTrackLeftBoundary = -125;
	const float sphere2MovementTrackRightBoundary = 75;
	//the max and base height that the balls can vertically move
	const float verticalMaxHeight = 40;
	const float verticalBaseHeight = 10;

	//the next six lines define the starting states for the sphere movement, sphere texture and there vertical state
	ESphereMovement sphere1State = Left;
	ESphereMovement sphere2State = Wait;
	ESphereMovement sphere3State = Wait;
	ESphereMovement sphere4State = Down;
	ESphere1Texture textureState = Violet;
	EVerticalMovement verticalState = Stationary;
	
		
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );
	
	myEngine->AddMediaFolder(".\\media");
	/**** Set up your scene here ****/

	//creates the floor and changes its texture to be the same as in the specification
	IMesh*floorMesh = myEngine->LoadMesh("floor.x");
	IModel*floor = floorMesh->CreateModel(0, -0.2, 0);
	floor->SetSkin("wood2.png");

	//creates the ingame board, each tile is 50units squared
	IMesh*boardMesh = myEngine->LoadMesh("board.x");
	IModel*board = boardMesh->CreateModel();

	//sets up the mannual camera so that it's in the correct location and at the correct angle
	ICamera*myCamera = myEngine->CreateCamera(kManual);
	myCamera->SetLocalPosition(0, 240, -240);
	myCamera->RotateLocalX(45);

	//creates the 4 spheres in the correct location with there starting textures
	IMesh*sphereMesh = myEngine->LoadMesh("ball.x");
	IModel*sphere1 = sphereMesh->CreateModel(125, 10, -125);
	sphere1->SetSkin("violet.png");
	IModel*sphere2 = sphereMesh->CreateModel(-125, 10, 75);
	sphere2->SetSkin("blue.png");
	IModel*sphere3 = sphereMesh->CreateModel(-125, 10, -125);
	sphere3->SetSkin("indigo.png");
	IModel* sphere4 = sphereMesh->CreateModel(125, 10, 75);
	sphere4->SetSkin("green.png");
	
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		
		//the first section of code is the sphere1 states
		if (sphere1State == Left)
		{
			sphere1->MoveX(-sphereSpeed);
			if (touchingFloor == true)//if statement checks if touching floor is true, if true then the balls will rotate towards their direction of travel
			{
				sphere1->RotateX(sphereSpeed);//the sphere will rotate at the same speed as the balls movement
			}
			if (sphere1->GetX() <= sphere1MovementTrackLeftBoundary)//if statement checks to see if the sphere has made it to the left boundary
			{
				sphere1->SetX(sphere1MovementTrackLeftBoundary);//this sets sphere1's location to the left boundary
				sphere1State = Up;//changes the spheres state from Left to Up
			}//the other three sphere1states complete the task as the Left state but with different boundarys
		}
		else if (sphere1State == Up)// the Up state
		{
			if (touchingFloor == true)
			{
				sphere1->RotateZ(sphereSpeed);
			}
			sphere1->MoveZ(sphereSpeed);
			if (sphere1->GetZ() >= sphereMovementUpBoundary)
			{
				sphere1->SetZ(sphereMovementUpBoundary);
				sphere1State = Right;
			}
		}
		else if (sphere1State == Right)// the right state
		{
			sphere1->MoveX(sphereSpeed);
			if (touchingFloor == true)
			{
				sphere1->RotateX(sphereSpeed);
			}
			if (sphere1->GetX() >= sphere1MovementTrackRightBoundary)
			{
				sphere1->SetX(sphere1MovementTrackRightBoundary);
				sphere1State = Down;
			}
		}
		else if (sphere1State == Down)// the down state
		{
			if (touchingFloor == true)
			{
				sphere1->RotateZ(-sphereSpeed);
			}
			sphere1->MoveZ(-sphereSpeed);
			if (sphere1->GetZ() <= sphereMovementDownBoundary)
			{
				sphere1->SetZ(sphereMovementDownBoundary);
				sphere1State = Wait;//sphere1 state changes to wait meaning it doesnt move
				sphere2State = Right;// sets sphere 2 to the Right state
				
			}
		}
		//-----------------------------------------------------
		//the section below is all of the sphere2 states
		if (sphere2State == Right)//sphere 2 has a different starthing point to sphere 1 hence why the first state is right
		{
			sphere2->MoveX(sphereSpeed);
			if (touchingFloor == true)//if statement checks if touching floor is true, if true then the balls will rotate towards their direction of travel
			{
				sphere2->RotateX(sphereSpeed);
			}
			if (sphere2->GetX() >= sphere2MovementTrackRightBoundary)//this ensure that sphere2 is at the right boundary
			{
				sphere2->SetX(sphere2MovementTrackRightBoundary);
				sphere2State = Down;
			}
		}
		else if (sphere2State == Down)
		{
			sphere2->MoveZ(-sphereSpeed);
			if (touchingFloor == true)
			{
				sphere2->RotateZ(-sphereSpeed);
			}
			if (sphere2->GetZ() <= sphereMovementDownBoundary)
			{
				sphere2->SetZ(sphereMovementDownBoundary);
				sphere2State = Left;
			}
		}
		else if (sphere2State == Left)
		{
			sphere2->MoveX(-sphereSpeed);
			if (touchingFloor == true)
			{
				sphere2->RotateX(-sphereSpeed);
			}
			if (sphere2->GetX() <= sphere2MovementTrackLeftBoundary)
			{
				sphere2->SetX(sphere2MovementTrackLeftBoundary);
				sphere2State = Up;
			}
		}
		else if (sphere2State == Up)
		{
			sphere2->MoveZ(sphereSpeed);
			if (touchingFloor == true)
			{
				sphere2->RotateZ(sphereSpeed);
			}
			if (sphere2->GetZ() >= sphereMovementUpBoundary)
			{
				sphere2->SetZ(sphereMovementUpBoundary);
				sphere2State = Wait;//this makes sphere 2 stationary
				sphere1State = Left;//this changes sphere1 to the left state
			}
		}
		//-------------------------------------------
		//the section below is the sphere3 states

		if (sphere3State == Up)
		{
			sphere3->MoveZ(sphereSpeed);
			if (touchingFloor == true)//if statement checks if touching floor is true, if true then the balls will rotate towards their direction of travel
			{
				sphere3->RotateZ(sphereSpeed);
			}
			if (sphere3->GetZ() >= sphereMovementUpBoundary)
			{
				sphere3->SetZ(sphereMovementUpBoundary);
				sphere3State = Right;
			}
		}
		else if (sphere3State == Right)
		{
			sphere3->MoveX(sphereSpeed);
			if (touchingFloor == true)
			{
				sphere3->RotateX(sphereSpeed);
			}
			if (sphere3->GetX() >= sphere2MovementTrackRightBoundary)
			{
				sphere3->SetX(sphere2MovementTrackRightBoundary);
				sphere3State = Down;
			}
		}
		else if (sphere3State == Down)
		{
			sphere3->MoveZ(-sphereSpeed);
			if (touchingFloor == true)
			{
				sphere3->RotateZ(-sphereSpeed);
			}
			if (sphere3->GetZ() <= sphereMovementDownBoundary)
			{
				sphere3->SetZ(sphereMovementDownBoundary);
				sphere3State = Left;
			}
		}
		else if (sphere3State == Left)
		{
			sphere3->MoveX(-sphereSpeed);
			if (touchingFloor == true)
			{
				sphere3->RotateX(-sphereSpeed);
			}
			if (sphere3->GetX() <= sphere2MovementTrackLeftBoundary)
			{
				sphere3->SetX(sphere2MovementTrackLeftBoundary);
				sphere3State = Wait;
				sphere4State = Down;
			}
		}
		//----------------------------------------------
		//the next section is sphere4 states
		if (sphere4State == Down)
		{
			sphere4->MoveZ(-sphereSpeed);
			if (touchingFloor == true)
			{
				sphere4->RotateZ(-sphereSpeed);
			}
			if (sphere4->GetZ() <= sphereMovementDownBoundary)
			{
				sphere4->SetZ(sphereMovementDownBoundary);
				sphere4State = Left;
			}
		}
		else if (sphere4State == Left)
		{
			sphere4->MoveX(-sphereSpeed);
			if (touchingFloor == true) //if statement checks if touching floor is true, if true then the balls will rotate towards their direction of travel
			{
				sphere4->RotateX(-sphereSpeed);
			}
			if (sphere4->GetX() <= sphere1MovementTrackLeftBoundary)
			{
				sphere4->SetX(sphere1MovementTrackLeftBoundary);
				sphere4State = Up;
			}
		}
		else if (sphere4State == Up)
		{
			sphere4->MoveZ(sphereSpeed);
			if (touchingFloor == true)
			{
				sphere4->RotateZ(sphereSpeed);
			}
			if (sphere4->GetZ() >= sphereMovementUpBoundary)
			{
				sphere4->SetZ(sphereMovementUpBoundary);
				sphere4State = Right;
			}
		}
		else if (sphere4State == Right)
		{
			sphere4->MoveX(sphereSpeed);
			if (touchingFloor == true)
			{
				sphere4->RotateX(sphereSpeed);
			}
			if (sphere4->GetX() >= sphere1MovementTrackRightBoundary)
			{
				sphere4->SetX(sphere1MovementTrackRightBoundary);
				sphere4State = Wait;
				sphere3State = Up;
			
			}
		}

		//-----------------------------------------------
		//this section of code is for getting the value of mouse wheel movement to change the speed
		wheelChange = myEngine->GetMouseWheelMovement();
		if (wheelChange == -1)
		{
			speedChange = speedChange - 0.1;//this decrease the speedChange multiplier by 1/10
			if (speedChange < minimumSpeed)//this checks to see if the value of speedChange has dropped beneath the minimum speed, if it has it executes the code beneath
			{
				speedChange = minimumSpeed;//this resets the speedchange back to the minimum speed
			}
			sphereSpeed = KGameSpeed * speedChange;//this recalculates the sphereSpeed
		}
		else if (wheelChange == 1)
		{
			speedChange = speedChange + 0.1;//this will increase the value of speedChange multiplier by 1/10
			if (speedChange > maximumSpeed)// this checks to see if speed change has gone past the maximum speed of 5, if it has it will execute the code below
			{
				speedChange = maximumSpeed;//this will reset the speed back to the maximum
			}
			sphereSpeed = KGameSpeed * speedChange;// this recalculates the sphereSpeed
		}

		//---------------------------------------------
		// the next section is the code for the escape key
		if (myEngine->KeyHit(Key_Escape))//this if statement checks to see if the escape key has been hit, when it has the code below is executed
		{
			myEngine->Stop();//this line of code will close the application
		}

		//-----------------------------------------------
		//this section is the code behind the P key for pausing the game
		if (myEngine->KeyHit(Key_P))//this if statement checks to see if the P key has been hit, when it has the code below is executed
		{
			if (isPaused == false)//this checks to see if the game has already been paused, if not it runs the code below
			{
				sphereSpeed = 0;//decreases the sphere speed to 0, this means that nothing can move
				isPaused = true;//sets the game to paused
			}
			else if (isPaused == true)//if it is paused then
			{
				sphereSpeed = KGameSpeed*speedChange;//recalculates the game speed
				isPaused = false;//sets the game to be unpaused 
			}
	    }
		
		//---------------------------------------------------
		//this section is the vertical movement of the spheres
		if (myEngine->KeyHit(Key_Up) && touchingFloor == true && verticalState == Stationary)//checks to see if the spheres are both touching the ground and stationary
		{
			touchingFloor = false;
			verticalState = VUP;//changes the verticalState to VUP
		}
		if (myEngine->KeyHit(Key_Down) && touchingFloor == false && verticalState == Stationary)//checks to see if the spheres are both no touching the ground and stationary
		{
			verticalState = VDown;//changes the verticalState to VDown
		}
		if (verticalState == VUP)//if the state is VUP then:
		{
			sphere1->MoveY(sphereYSpeed);//increased the Y axis of all 4 spheres 
			sphere2->MoveY(sphereYSpeed);
			sphere3->MoveY(sphereYSpeed);
			sphere4->MoveY(sphereYSpeed);
			if (sphere1->GetY() >= verticalMaxHeight)//checks to see if there at max height 
			{
				sphere1->SetY(verticalMaxHeight);//sets them to max height incase they overshot but that shouldnt happpen
				sphere2->SetY(verticalMaxHeight);
				sphere3->SetY(verticalMaxHeight);
				sphere4->SetY(verticalMaxHeight);
				verticalState = Stationary;//sets the state to stationary
			}
		}
		if (verticalState == VDown)//does the same as previous state but in reverse
		{
			sphere1->MoveY(-sphereYSpeed);
			sphere2->MoveY(-sphereYSpeed);
			sphere3->MoveY(-sphereYSpeed);
			sphere4->MoveY(-sphereYSpeed);
			if (sphere1->GetY() <= verticalBaseHeight)
			{
				sphere1->SetY(verticalBaseHeight);
				sphere2->SetY(verticalBaseHeight);
				sphere3->SetY(verticalBaseHeight);
				sphere4->SetY(verticalBaseHeight);
				verticalState = Stationary;
				touchingFloor = true;
			}
		}
		
		//------------------------------------------------
		//this section looks at the colour change
		if (myEngine->KeyHit(Key_E))
		{
			if (textureState == Violet)
			{
				sphere1->SetSkin("violet.png");//this sets the texture of the sphere in a clockwise direction
				sphere2->SetSkin("blue.png");
				sphere3->SetSkin("indigo.png");
				sphere4->SetSkin("green.png");
				textureState = Blue;//changes the state to the next one in the sequence
			}
			else if (textureState == Blue)
			{
				sphere1->SetSkin("green.png");
				sphere2->SetSkin("indigo.png");
				sphere3->SetSkin("violet.png");
				sphere4->SetSkin("blue.png");
				textureState = Indigo;
			}
			else if (textureState == Indigo)
			{
				sphere1->SetSkin("blue.png");
				sphere2->SetSkin("violet.png");
				sphere3->SetSkin("green.png");
				sphere4->SetSkin("indigo.png");
				textureState = Green;
			}
			else if (textureState == Green)
			{
				sphere1->SetSkin("indigo.png");
				sphere2->SetSkin("green.png");
				sphere3->SetSkin("blue.png");
				sphere4->SetSkin("violet.png");
				textureState = Violet;
			}
		}
    }
		// Delete the 3D engine now we are finished with it
		myEngine->Delete();
	
}
