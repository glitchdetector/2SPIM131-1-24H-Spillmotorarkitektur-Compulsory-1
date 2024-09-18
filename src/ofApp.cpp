#include "ofApp.h"

#include <GLFW/glfw3.h>

float scale = 1.0;
float gravity = 0.2f;
float friction = 1.0f;
float impactFriction = 0.9f;
ofVec2f terminalVelocity = ofVec2f(4.f, 4.f);

enum ECollisionShapes
{
	SHAPE_CIRCLE,
	SHAPE_BALL = SHAPE_CIRCLE,
	SHAPE_SPHERE = SHAPE_CIRCLE,
	SHAPE_RADIUS = SHAPE_CIRCLE,
	SHAPE_BOX,
	SHAPE_CUBE = SHAPE_BOX,
	SHAPE_RECTANGLE = SHAPE_BOX,
	SHAPE_SQUARE = SHAPE_BOX,
	SHAPE_WALL,
	SHAPE_FLAT = SHAPE_WALL,
	SHAPE_PLANE = SHAPE_WALL,
};

// Generic SceneObject, encapsulates everything in the scene
class SceneObject {
public:
	virtual void so() {};
	string name{ "Scene Object" };
};

// Collision class, all physics objects and impacting objects use this
class Collision : public SceneObject
{
public:
	ECollisionShapes collisionShape{ ECollisionShapes::SHAPE_BOX };
	float width{ 1.f };
	float height{ 1.f };
};

// Static wall with a flat shape and one-directional collision
class Wall : public Collision {
public:
	Wall(ofVec2f start, ofVec2f end)
	{
		collisionShape = ECollisionShapes::SHAPE_WALL;
	}
	ofVec2f start;
	ofVec2f end;
};

// A physics controlled actor in the scene
class Entity : public Collision {
public:
	ofVec2f position;
	ofVec2f velocity;
	float mass{ 1.0f };
};

// Round bouncy ball
class Ball : public Entity {
public:
	Ball(ofVec2f position, float radius = 16.f, float mass = 1.f)
	{
		this->position = position;
		this->width = radius;
		this->mass = mass;
		this->collisionShape = ECollisionShapes::SHAPE_RADIUS;
	}
};

// Holds all information in the current scene
class Scene
{
public:
	std::vector<SceneObject*> objects;
	std::vector<Ball*> getBalls() {
		std::vector<Ball*> balls;
		for (SceneObject* object : objects)
		{
			if (Ball* ball = dynamic_cast<Ball*>(object)) {
				balls.push_back(ball);
			}
		}
		return balls;
	}
	Ball* firstBall()
	{
		return this->getBalls().front();
	}
};

// Ball/Ball collision algorithm from lecture
float ballBallCollision(Ball* p, Ball* q)
{
	static const float EPSILON{ 0.000001 };
	auto A = p->position - q->position;
	auto B = p->velocity - q->velocity;
	auto AA = A.dot(A);
	auto AB = A.dot(B);
	auto BB = B.dot(B);
	auto d = p->width + q->width;
	auto rot = AB * AB - BB * (AA - d * d);
	float t{-1.0f};
	if (rot >= 0.0f)
		t = -AB - sqrtf(rot);
	if (BB > EPSILON)
		t = t / BB;
	return t;
}

// Ball/ball collision response from lecture
std::pair<ofVec2f, ofVec2f> collisionResponse(Ball* p, Ball* q)
{
	ofVec2f v0 = p->velocity * (p->mass - q->mass) + (q->velocity * q->mass * 2);
	v0 /= (p->mass + q->mass);
	ofVec2f v1 = q->velocity * (q->mass - p->mass) + (p->velocity * p->mass * 2);
	v1 /= (q->mass + p->mass);
	return { v0, v1 };
}

// External boundaries of the window, balls will bounce off these perimeters
ofRectangle boundaries = ofRectangle(50, 50, ofApp::WINDOW_WIDTH - 100, ofApp::WINDOW_HEIGHT - 100);

// Scene object, contains all the entities in the scene
Scene* scene = new Scene;

//--------------------------------------------------------------
void ofApp::setup(){

	// Randomly generate new balls
	for (int i = 0; i < nBalls; i++)
	{
		float radius = 8.f + ofRandomuf() * 32.f;

		// This mass value is just based on test values, where 16 radius = ~1f, 32 radius is ~4f and 8 radius is ~.25f
		// Formula reverse engineered based on those defaults
		float mass = 0.00024414f * pow(radius, 3);

		scene->objects.push_back(new Ball({ ofVec2f(.0f, .0f), radius, mass }));

	}

	// Randomly place the balls, may overlap with others
	// TODO: settle balls before simulation
	for (Ball* ball : scene->getBalls())
	{
		ball->position = ofVec2f(50.f, 50.f)
		+ ofVec2f(abs(ofRandomf()) * ofApp::WINDOW_WIDTH, abs(ofRandomf()) * ofApp::WINDOW_HEIGHT);
	}

	std::cout << "length: " << scene->objects.size() << std::endl;
	ofDisableDepthTest();
}

float cursorX{ 0.0f }, cursorY{ 0.0f };

//--------------------------------------------------------------
void ofApp::update() {
	for (Ball* ball : scene->getBalls())
	{

		// Check collision with balls
		for (Ball* oBall : scene->getBalls())
		{
			if (ball == oBall) continue;
			float collision = ballBallCollision(ball, oBall);
			if (0 < collision && collision < 1)
			{
				std::cout << "collision" << std::endl;
				// Collision detected
				std::pair<ofVec2f, ofVec2f> result = collisionResponse(ball, oBall);
				ball->velocity = result.first;
				oBall->velocity = result.second;
			}
		}

		// Boundary collision check
		if (ball->position.x < boundaries.getLeft() + ball->width)
		{
			ball->position.x = boundaries.getLeft() + ball->width;
			ball->velocity.x *= -1;
		}
		if (ball->position.x > boundaries.getRight() - ball->width)
		{
			ball->position.x = boundaries.getRight() - ball->width;
			ball->velocity.x *= -1;
		}
		if (ball->position.y < boundaries.getTop() + ball->width)
		{
			ball->position.y = boundaries.getTop() + ball->width;
			ball->velocity.y *= -1;
		}
		if (ball->position.y > boundaries.getBottom() - ball->width)
		{
			ball->position.y = boundaries.getBottom() - ball->width;
			ball->velocity.y *= -1;
		}

	}

	// Move all the balls
	for (Ball* ball : scene->getBalls())
	{
		ball->position += ball->velocity;
	}

}

// Convert a grid coordinate to screen coords
ofVec2f gridPos(int x, int y)
{
	return ofVec2f(0 + x * 1 * scale, 0 + y * 1 * scale);
}

// Draw a line
void ln(int fromX, int fromY, int toX, int toY)
{
	ofVec2f from = gridPos(fromX, fromY);
	ofVec2f to = gridPos(toX, toY);
	ofDrawLine(from, to);
}

void rec(int x, int y, int w, int h)
{
	ln(x, y, x + w, y);
	ln(x, y, x, y + h);
	ln(x + w, y, x + w, y + h);
	ln(x, y + h, x + w, y + h);
}

void ofApp::draw(){
	rec(boundaries.x, boundaries.y, boundaries.width, boundaries.height);
	bool firstBall = true;
	for (SceneObject* object : scene->objects)
	{
		ofSetColor(255, 255, 255);
		if (Ball* ball = dynamic_cast<Ball*>(object))
		{
			if (firstBall)
			{
				firstBall = false;
				ofSetColor(255, 0, 0);
			}
			ofDrawCircle(ball->position.x, ball->position.y, ball->width);
		}
		if (Wall* wall = dynamic_cast<Wall*>(object))
		{
			ln(wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		}
	}
	if (Ball* target = scene->firstBall())
	{
		if (target->velocity.length() == 0.f)
		{
			ofVec2f offset = target->position - ofVec2f(mouseX, mouseY);
			ln(target->position.x, target->position.y, target->position.x + offset.x, target->position.y + offset.y);
		}
	}
}


void ofApp::keyPressed(int key){
	if (key == GLFW_KEY_SPACE)
	{
		for (Ball* ball : scene->getBalls())
		{
			ball->velocity = ofVec2f(-10.f, -2.f);
			break;
		}
	}
}

void ofApp::mousePressed(int x, int y, int button){
	if (Ball* target = scene->firstBall())
	{
		if (target->velocity.length() == 0.f)
			target->velocity = (target->position - ofVec2f(x, y)).normalize() * 5.f;
	}
}