#include "BodyShape_ph.h"

BodyShape_ph::BodyShape_ph()
{
	vertices = std::make_unique<b2Vec2[]>(V);
}


BodyShape_ph::~BodyShape_ph()
{
}

void BodyShape_ph::createBodyShape(b2World & world, float x, float y)
{
	b2BodyDef polygonDefinition;
	polygonDefinition.type = b2_dynamicBody;
	polygonDefinition.position.Set(x, y);
	b2Body * polygon = world.CreateBody(&polygonDefinition);

	b2PolygonShape polygonShape;
	
	polygonShape.Set(this->getVertices(), V);

	b2FixtureDef polygonFixture;
	polygonFixture.shape = &polygonShape;
	polygonFixture.density = this->getDensity();
	polygonFixture.friction = 0.3f;
	polygonFixture.restitution = 0.0;
	polygonFixture.filter.categoryBits = 0x0002;
	polygonFixture.filter.maskBits = 0x0001;

	polygon->CreateFixture(&polygonFixture);

	this->setBody(polygon);
}



void BodyShape_ph::setVertices(std::vector<std::unique_ptr<b2Vec2>> const & tab)
{
	for (int i = 0; i < V; ++i) {
		vertices[i].x = tab[i]->x;
		vertices[i].y = tab[i]->y;
	}
}

void BodyShape_ph::setVert(std::vector<ShapePoint> vec)
{
	for (int i = 0; i < vec.size(); ++i) {
		vertices[i].x = vec[i].getX();
		vertices[i].y = vec[i].getY();
	}
}

b2Vec2 * BodyShape_ph::getVertices()
{
	return vertices.get();
}
