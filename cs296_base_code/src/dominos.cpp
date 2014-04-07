/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

/* 
 * Base code for CS 296 Software Systems Lab 
 * Department of Computer Science and Engineering, IIT Bombay
 * Instructor: Parag Chaudhuri
 */


#include "cs296_base.hpp"
#include "render.hpp"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

#include <cstring>
using namespace std;

#include "dominos.hpp"

namespace cs296
{
  /**  The is the constructor 
   * This is the documentation block for the constructor.
   */ 
  
  dominos_t::dominos_t()
  {
    //Ground
    /*! \var b1 
     * \brief pointer to the body ground 
     */ 
      
     short category1 = 0x0001;
     short category2 = 0x0002;
     short category3 = 0x0003;
//category 4 and 5 used
     short groundCategory = 0x0006;
     
     short mask1 = -1;
     short mask2 = category1 ;
     short mask3 = category1 ;

	 float x,y,angle;

     //groundBody
     const float degtorad = 0.01745329251994f;
    b2Body* groundBody;
    {
	    b2EdgeShape groundBodyShape; 
	    b2FixtureDef groundBodyFixDef;
	    groundBodyFixDef.filter.categoryBits = groundCategory;
	    groundBodyFixDef.filter.maskBits = 0;
	    groundBodyFixDef.filter.groupIndex = -5;
	    groundBodyShape.Set(b2Vec2(-0.1f, 100.0f), b2Vec2(0.1f, 100.0f));
	    b2BodyDef bd; 
	    groundBody = m_world->CreateBody(&bd); 
	    groundBody->CreateFixture(&groundBodyShape, 0.0f);
	}
	    

    {//left piston side
    	b2Body* b1;
		b2PolygonShape bodyPoly;
		b2BodyDef bd;
		bodyPoly.SetAsBox(2.0f, 10.0f );
		b2FixtureDef fd;
			
		fd.filter.categoryBits = category1;
		fd.filter.maskBits = mask1;
		fd.shape = &bodyPoly;
		fd.restitution = 1.0f;
		fd.friction = 2.0;
		bd.position.Set(-10.0f, 10.0f);
		b1 = m_world->CreateBody(&bd);
		b1->CreateFixture(&fd);
    }

    {//right piston side
    	b2Body* b1;
		b2PolygonShape bodyPoly;
		b2BodyDef bd;
		bodyPoly.SetAsBox(2.0f, 10.0f );
		b2FixtureDef fd;
		
		fd.filter.categoryBits = category1;
		fd.filter.maskBits = mask1;
		fd.shape = &bodyPoly;
		fd.restitution = 1.0f;
		fd.friction = 2.0;
		bd.position.Set(10.0f, 10.0f);
		b1 = m_world->CreateBody(&bd);
		b1->CreateFixture(&fd);
    }

    {//top piston side
		  b2Vec2 vertices[3];
		  vertices[0].Set(-8+2*sqrt(2), -2);
		  vertices[1].Set(-5, 1);
		  vertices[2].Set(5,  1);
		  vertices[3].Set(8-2*sqrt(2), -2);
		  
		  b2PolygonShape polygonShape;
		  polygonShape.Set(vertices, 4); //pass array to the shape
		  
		  b2FixtureDef myFixtureDef;
		  myFixtureDef.filter.groupIndex = -1;
		  myFixtureDef.filter.categoryBits = category1;
		  myFixtureDef.filter.maskBits = mask1;
		  b2BodyDef myBodyDef;
		  myFixtureDef.shape = &polygonShape; //change the shape of the fixture
		  myFixtureDef.friction = 2.0;
		  myFixtureDef.restitution = 1.0;
		  myBodyDef.position.Set(0 ,25); //in the middle
		  //myBodyDef.type=b2_dynamicBody;
		  b2Body* dynamicBody1 = m_world->CreateBody(&myBodyDef);
		  dynamicBody1->CreateFixture(&myFixtureDef); //add a fixture to the body

    }

    
	{//Fuel control(have added the control rod )
		   b2Vec2 vertices[3];
		  vertices[0].Set(0.0f, -2.0f);
		  vertices[1].Set(2.8f, -3.7f);
		  vertices[2].Set(-2.8f, -3.7f);
		  b2PolygonShape shape;
		  shape.Set(vertices, 3); //pass array to the shape

		  b2PolygonShape shape2;
		  shape2.SetAsBox(0.3f, 2.1f);
		  b2FixtureDef *fd = new b2FixtureDef;
		  fd->shape = new b2PolygonShape;
		  fd->shape = &shape;
		  fd->density = 0.1f;
		  fd->friction = 1.0f;
		  fd->filter.categoryBits = category1;
		  fd->filter.maskBits = mask1;


		  b2BodyDef bd2;
		  bd2.position.Set(8.0f, 23.0f);
		  bd2.angle = -45 * degtorad;
		  bd2.type = b2_dynamicBody;
		  b2Body* body2 = m_world->CreateBody(&bd2);
		  b2FixtureDef *fd2 = new b2FixtureDef;
		  fd2->density = 0.1f;
		  fd2->shape = new b2PolygonShape;
		  fd2->shape = &shape2;
		  fd2->filter.categoryBits = category1;
		  fd2->filter.maskBits = mask1;
		  body2->CreateFixture(fd2);
		  body2->CreateFixture(fd);

		  b2PrismaticJointDef prismaticJointDef;
		  b2Vec2 v;
		  v.Set(1, 1);
		  prismaticJointDef.Initialize(body2, groundBody, body2->GetWorldCenter(), v);
		  m_world->CreateJoint(&prismaticJointDef);
		  
	////////////////////////
	    //Fuel Control Rod right
	b2Body* fuelRod1;
    {
		b2PolygonShape fuelRod1Poly;
		b2BodyDef fuelRod1Def;
		fuelRod1Poly.SetAsBox(4, 0.6f);
		b2FixtureDef fuelRod1FixDef;

		fuelRod1FixDef.filter.categoryBits = category3;
		fuelRod1FixDef.filter.maskBits = mask3;
		fuelRod1FixDef.shape = & fuelRod1Poly;
		fuelRod1FixDef.density = 10.0f;
		fuelRod1FixDef.restitution = 1.0f;
		fuelRod1Def.position.Set(7, 27);
		fuelRod1Def.type = b2_dynamicBody;
		fuelRod1Def.angle = -35 * degtorad;
		fuelRod1 = m_world->CreateBody(&fuelRod1Def);
		fuelRod1->CreateFixture(&fuelRod1FixDef);
		
		b2RevoluteJointDef* revolutejd = new b2RevoluteJointDef();
		revolutejd->bodyA = fuelRod1;
		revolutejd->bodyB = groundBody;
		revolutejd->collideConnected = false;
		revolutejd->localAnchorA.Set(0,0); 
		revolutejd->localAnchorB = fuelRod1->GetWorldCenter();
		
		
		m_world->CreateJoint(revolutejd);
		
		
	}	  

	b2DistanceJointDef distancejoint;
	b2Vec2 anchor1, anchor2;
	anchor1.Set(-(-8+ 2*sqrt(2)-6*cos(45* degtorad)),23- 2*sqrt(2)+6*sin(45* degtorad));
	anchor2.Set(-(-7-4*cos(35* degtorad)), 27-4*sin(35 * degtorad) );
	distancejoint.Initialize(body2, fuelRod1, anchor1, anchor2);
	m_world->CreateJoint(&distancejoint);
	}


	
	{//Exhaust control(have added the control rod )
		   b2Vec2 vertices[3];
		  vertices[0].Set(0.0f, -2.0f);
		  vertices[1].Set(-2.8f, -3.7f);
		  vertices[2].Set(2.8f, -3.7f);
		  b2PolygonShape shape;
		  shape.Set(vertices, 3); //pass array to the shape

		  b2PolygonShape shape2;
		  shape2.SetAsBox(0.3f, 2.1f);
		  b2FixtureDef *fd = new b2FixtureDef;
		  fd->shape = new b2PolygonShape;
		  fd->shape = &shape;
		  fd->density = 0.1f;
		  fd->friction = 1.0f;
		  fd->filter.categoryBits = category1;
		  fd->filter.maskBits = mask1;


		  b2BodyDef bd2;
		  bd2.position.Set(-8.0f, 23.0f);
		  bd2.angle = 45 * degtorad;
		  bd2.type = b2_dynamicBody;
		  b2Body* body2 = m_world->CreateBody(&bd2);
		  b2FixtureDef *fd2 = new b2FixtureDef;
		  fd2->density = 0.1f;
		  fd2->shape = new b2PolygonShape;
		  fd2->shape = &shape2;
		  fd2->filter.categoryBits = category1;
		  fd2->filter.maskBits = mask1;
		  body2->CreateFixture(fd2);
		  body2->CreateFixture(fd);

		  b2PrismaticJointDef prismaticJointDef;
		  b2Vec2 v;
		  v.Set(1, -1);
		  prismaticJointDef.Initialize(body2, groundBody, body2->GetWorldCenter(), v);
		  m_world->CreateJoint(&prismaticJointDef);
		  
	////////////////////////
	    //Exhaust Control Rod left
	b2Body* fuelRod1;
    {
		b2PolygonShape fuelRod1Poly;
		b2BodyDef fuelRod1Def;
		fuelRod1Poly.SetAsBox(4, 0.6f);
		b2FixtureDef fuelRod1FixDef;

		fuelRod1FixDef.filter.categoryBits = category2;
		fuelRod1FixDef.filter.maskBits = mask2 | category2;
		fuelRod1FixDef.shape = & fuelRod1Poly;
		fuelRod1FixDef.density = 10.0f;
		fuelRod1FixDef.restitution = 1.0f;
		fuelRod1Def.position.Set(-7, 27);
		fuelRod1Def.type = b2_dynamicBody;
		fuelRod1Def.angle = 35 * degtorad;
		fuelRod1 = m_world->CreateBody(&fuelRod1Def);
		fuelRod1->CreateFixture(&fuelRod1FixDef);
		
		b2RevoluteJointDef* revolutejd = new b2RevoluteJointDef();
		revolutejd->bodyA = fuelRod1;
		revolutejd->bodyB = groundBody;
		revolutejd->collideConnected = false;
		revolutejd->localAnchorA.Set(0,0); 
		revolutejd->localAnchorB = fuelRod1->GetWorldCenter();
		
		
		m_world->CreateJoint(revolutejd);
		
		
	}	  

	b2DistanceJointDef distancejoint;
	b2Vec2 anchor1, anchor2;
	anchor1.Set(-8+ 2*sqrt(2)-6*cos(45* degtorad),23- 2*sqrt(2)+6*sin(45* degtorad));
	anchor2.Set(-7-4*cos(35* degtorad), 27-4*sin(35 * degtorad) );
	distancejoint.Initialize(body2, fuelRod1, anchor1, anchor2);
	m_world->CreateJoint(&distancejoint);
	}
	
	//planks in middle
b2Body* fuelPlank;
	{//fuel part
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(2.5, 0.1);
        	b2FixtureDef myFixtureDef;
	        b2BodyDef myBodyDef;		

		myBodyDef.type=b2_dynamicBody;
		myBodyDef.angle = -15*degtorad;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;
		myFixtureDef.restitution = 0.1;
		myFixtureDef.filter.categoryBits = category3;
		myFixtureDef.filter.maskBits = mask3;
		myFixtureDef.filter.groupIndex = -1;
		myBodyDef.position.Set(2.5,29); 
		b2Body* fuelPlank = m_world->CreateBody(&myBodyDef);
		fuelPlank->CreateFixture(&myFixtureDef); 
		
		b2RevoluteJointDef* revolutejd = new b2RevoluteJointDef();
		revolutejd->bodyA = fuelPlank;
		revolutejd->bodyB = groundBody;
		revolutejd->collideConnected = false;
		revolutejd->localAnchorA.Set(-2.5,0); 
		revolutejd->localAnchorB.Set(0,29);
		revolutejd->enableMotor = true;
		revolutejd->motorSpeed = -2;
		revolutejd->maxMotorTorque = 1000;
		m_world->CreateJoint(revolutejd);
	}
b2Body* exhaustPlank;	
b2Joint* exhaustCtrlJoint;
	{//exhaust part
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(2.5, 0.1);
		b2FixtureDef myFixtureDef;
	        b2BodyDef myBodyDef;
		

		myBodyDef.type=b2_dynamicBody;
		myBodyDef.angle = 15*degtorad;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;
		myFixtureDef.restitution = 0.1;
		myFixtureDef.filter.categoryBits = category2;
		myFixtureDef.filter.maskBits = mask2 | category2;
		myFixtureDef.filter.groupIndex = -1;
		myBodyDef.position.Set(-2.5,29); 
		exhaustPlank = m_world->CreateBody(&myBodyDef);
		exhaustPlank->CreateFixture(&myFixtureDef);


		b2RevoluteJointDef* revolutejd = new b2RevoluteJointDef();
		revolutejd->bodyA = exhaustPlank;
		revolutejd->bodyB = groundBody;
		revolutejd->collideConnected = false;
		revolutejd->localAnchorA.Set(2.5,0); 
		revolutejd->localAnchorB.Set(0,29);
		revolutejd->enableMotor = true;
		revolutejd->motorSpeed = 2;
		revolutejd->maxMotorTorque = 1000;
		exhaustCtrlJoint = m_world->CreateJoint(revolutejd);
	}

	{//testing with small balls
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(7, 0.1);
		b2FixtureDef myFixtureDef;
		b2BodyDef myBodyDef;
		myBodyDef.angle = 45*degtorad;
		myFixtureDef.shape = &polygonShape; 
		myFixtureDef.density = 100;
		myFixtureDef.restitution = 1;
		myFixtureDef.filter.categoryBits = 0x0004;
		myFixtureDef.filter.maskBits = 0x0005;
		myBodyDef.position.Set(10,29);
		b2Body* Body = m_world->CreateBody(&myBodyDef);
		Body->CreateFixture(&myFixtureDef); //add a fixture to the body




		b2PolygonShape polygonShape1;
		polygonShape1.SetAsBox(7, 0.1);
		b2FixtureDef myFixtureDef1;
		b2BodyDef myBodyDef1;
		//myBodyDef1.type=b2_dynamicBody;
		myBodyDef1.angle = 45*degtorad;
		//myBodyDef.linearVelocity.Set(0,5);
		myFixtureDef1.shape = &polygonShape1; //change the shape of the fixture
		myFixtureDef1.density = 100;
		myFixtureDef1.restitution = 1;
		myFixtureDef1.filter.categoryBits = 0x0004;
		myFixtureDef1.filter.maskBits = 0x0005;
		//myFixtureDef.filter.groupIndex = -1;
		myBodyDef1.position.Set(14,25);
		b2Body* Body1 = m_world->CreateBody(&myBodyDef1);
		Body1->CreateFixture(&myFixtureDef1); //add a fixture to the body
		{
		b2PolygonShape polygonShape1;
		polygonShape1.SetAsBox(5, 0.1);
			//polygonShape.Set(vertices, 5); //pass array to the shape

		b2FixtureDef myFixtureDef1;
		b2BodyDef myBodyDef1;
		//myBodyDef1.type=b2_dynamicBody;
		myBodyDef1.angle = 135*degtorad;
		//myBodyDef.linearVelocity.Set(0,5);
		myFixtureDef1.shape = &polygonShape1; //change the shape of the fixture
		myFixtureDef1.density = 100;
		myFixtureDef1.restitution = 0.1;
		myFixtureDef1.filter.categoryBits = 0x0004;
		myFixtureDef1.filter.maskBits = 0x0005;
		//myFixtureDef.filter.groupIndex = -1;
		myBodyDef1.position.Set(14,32);
		b2Body* Body1 = m_world->CreateBody(&myBodyDef1);
		Body1->CreateFixture(&myFixtureDef1); //add a fixture to the body
		}


		b2Body* spherebody;
		{	
		b2CircleShape circle;
		circle.m_radius = 0.4;

		b2FixtureDef ballfd;
		ballfd.shape = &circle;
		ballfd.density = 0.0000007f;
		ballfd.friction = 0.0f;
		
		ballfd.restitution = 1.0f;
		
		for (int i = 0; i < 50; ++i)
		{
			b2BodyDef ballbd;
			ballbd.type = b2_dynamicBody;
			ballbd.position.Set((rand() % 2) + 12.0f , (rand() % 2) + 27.0f);
			ballbd.linearVelocity.Set((rand() % 10) - 5.0f , (rand() % 10) - 5.0f);
			ball_array[i] = m_world->CreateBody(&ballbd);
			//spherebody->userData = 1;
			ball_array[i]->CreateFixture(&ballfd);
		}
			ballfd.density = 0.00007f;
			b2BodyDef ballbd;
			//ballbd.type = b2_dynamicBody;
			ballbd.position.Set(0,20);
			//ballbd.linearVelocity.Set((rand() % 10) - 5.0f , (rand() % 10) - 5.0f);
			spherebody = m_world->CreateBody(&ballbd);
			spherebody->CreateFixture(&ballfd);
		
	}
}
b2Body* pistonSphere;
b2Joint* sphereGndJoint;

	{
		b2Body* piston;
		{
			b2PolygonShape polygonShape1;
			polygonShape1.SetAsBox(7.9, 2);

			b2FixtureDef myFixtureDef1;
			b2BodyDef myBodyDef1;
			myBodyDef1.type=b2_dynamicBody;
			//myBodyDef1.angle = 135*degtorad;
			myBodyDef1.linearVelocity.Set(0,-4);
			myFixtureDef1.shape = &polygonShape1; //change the shape of the fixture
			myFixtureDef1.density = 0.0001;
			myFixtureDef1.restitution = 1;
			myFixtureDef1.filter.categoryBits = 0x0004;
			myFixtureDef1.filter.groupIndex = -5;
			myBodyDef1.position.Set(0,8);
			piston = m_world->CreateBody(&myBodyDef1);
			piston->CreateFixture(&myFixtureDef1); //add a fixture to the body
		}
		b2Body* pistonRod;
		{
			b2PolygonShape polygonShape1;
			polygonShape1.SetAsBox(1, 2.5f*sqrt(10));

			b2FixtureDef myFixtureDef1;
			b2BodyDef myBodyDef1;
			myBodyDef1.type=b2_dynamicBody;
			myBodyDef1.angle = - atan(1/3.0f);
			//myBodyDef.linearVelocity.Set(0,5);
			myFixtureDef1.shape = &polygonShape1; //change the shape of the fixture
			myFixtureDef1.density = 0.0001;
			myFixtureDef1.restitution = 1;
			//myFixtureDef1.filter.categoryBits = 0x0004;
			//myFixtureDef1.filter.maskBits = 0x0005;
			myFixtureDef1.filter.groupIndex = -5;
			myBodyDef1.position.Set(-2.5f,0.5f);
			pistonRod = m_world->CreateBody(&myBodyDef1);
			pistonRod->CreateFixture(&myFixtureDef1); //add a fixture to the body
		}
		b2RevoluteJointDef* revolutejd = new b2RevoluteJointDef();
		revolutejd->bodyA = piston;
		revolutejd->bodyB = pistonRod;
		revolutejd->collideConnected = false;
		revolutejd->localAnchorA.Set(0,0); 
		revolutejd->localAnchorB.Set(0,7.5);
		//revolutejd->collideConnected = false;
		m_world->CreateJoint(revolutejd);

		
			{	
			b2CircleShape circle;
			circle.m_radius = 5.5;

			b2FixtureDef ballfd;
			ballfd.shape = &circle;
			ballfd.density = 0.0001f;
			ballfd.friction = 0.0f;
			ballfd.restitution = 1.0f;
			ballfd.filter.groupIndex = -5;
			b2BodyDef ballbd;
			ballbd.type = b2_dynamicBody;
			ballbd.position.Set(0, -7);
			pistonSphere = m_world->CreateBody(&ballbd);
			pistonSphere->CreateFixture(&ballfd);

		}
		
		b2RevoluteJointDef* jointRodSphereDef = new b2RevoluteJointDef();
		jointRodSphereDef->bodyA = pistonSphere;
		jointRodSphereDef->bodyB = pistonRod;
		jointRodSphereDef->collideConnected = false;
		jointRodSphereDef->localAnchorA.Set(-5,0); 
		jointRodSphereDef->localAnchorB.Set(0,-7.5);
		//jointRodSphereDef->collideConnected = false;
		m_world->CreateJoint(jointRodSphereDef);

		b2RevoluteJointDef* jointGndSphereDef = new b2RevoluteJointDef();
		jointGndSphereDef->bodyA = pistonSphere;
		jointGndSphereDef->bodyB = groundBody;
		jointGndSphereDef->collideConnected = false;
		jointGndSphereDef->localAnchorA.Set(0,0); 
		jointGndSphereDef->localAnchorB = pistonSphere->GetWorldCenter();
		//jointGndSphereDef->collideConnected = false;
		sphereGndJoint = m_world->CreateJoint(jointGndSphereDef);

		b2PrismaticJointDef prismaticJointDef;
 		b2Vec2 v;
		v.Set(0, 1);
		prismaticJointDef.Initialize(piston, groundBody, piston->GetWorldCenter(), v);
		m_world->CreateJoint(&prismaticJointDef);

	}
	
}
sim_t *sim = new sim_t("Dominos", dominos_t::create);



void dominos_t::step(settings_t* settings){
		base_sim_t::step(settings);
		
		for(int i = 0; i < 50; i++){
			b2Vec2 pos = ball_array[i]->GetPosition();
			b2Vec2 vel = ball_array[i]->GetLinearVelocity();
			if(pos.y >40 || pos.x < -30 || pos.y<0){
		ball_array[i]->SetTransform(b2Vec2((rand() % 2) + 12.0f, (rand() % 2) + 27.0f), 0);
		ball_array[i]->SetLinearVelocity(b2Vec2((rand() % 10) - 5.0f , (rand() % 10) - 5.0f));
		ball_array[i]->GetFixtureList()->SetDensity( 0.0000007f);
		}
		
		
	}
}
}
