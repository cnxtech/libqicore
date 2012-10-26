/**
* @author Aldebaran Robotics
* Aldebaran Robotics (c) 2012 All Rights Reserved
*/

#include <newframemanager/diagram.h>
#include "diagram_private.h"

DiagramPrivate::DiagramPrivate()
  : _boxes()
{
}

DiagramPrivate::~DiagramPrivate()
{
  for (std::set<Box*>::iterator it = _boxes.begin();
        it != _boxes.end(); it++)
  {
    delete (*it);
  }
}

void DiagramPrivate::addBox(Box *b)
{
  _boxes.insert(b);
}

void DiagramPrivate::removeBox(Box *b)
{
  _boxes.erase(b);
}

void DiagramPrivate::loadAll()
{
  for (std::set<Box*>::iterator it = _boxes.begin();
        it != _boxes.end(); it++)
  {
    loadBox(*it);
  }
}

void DiagramPrivate::unloadAll()
{
  for (std::set<Box*>::iterator it = _boxes.begin();
        it != _boxes.end(); it++)
  {
    unloadBox(*it);
  }
}

void DiagramPrivate::loadFromDiagram(Diagram* d)
{
  std::set<Box*>& l = d->_p->_boxes;
  std::set<Box*> toLoad = _boxes;

  for (std::set<Box*>::iterator it = l.begin();
        it != l.end(); it++)
  {
    std::set<Box*>::iterator loadIt = toLoad.find(*it);

    if (loadIt == toLoad.end())
      unloadBox(*it);
    else
      toLoad.erase(loadIt);
  }

  for (std::set<Box*>::iterator it = toLoad.begin();
        it != toLoad.end(); it++)
  {
    loadBox(*it);
  }
}

void DiagramPrivate::loadBox(Box* b)
{
  AL::ALValue params = AL::ALValue();
  AL::ALValue result = AL::ALValue();
  b->executePython("__onLoad__", params, result);
  // Useless ?
  //b->executePython("onInput_onLoad__", params, result);
}

void DiagramPrivate::unloadBox(Box* b)
{
  AL::ALValue params = AL::ALValue();
  AL::ALValue result = AL::ALValue();
  b->executePython("__onUnload__", params, result);
}

void DiagramPrivate::merge(Diagram* d)
{
  std::set<Box*>& l = d->_p->_boxes;

  _boxes.insert(l.begin(), l.end());
}

/* Public */
Diagram::Diagram()
  : _p (new DiagramPrivate())
{
}

Diagram::~Diagram()
{
  delete _p;
}

void Diagram::addBox(Box *b)
{
  _p->addBox(b);
}

void Diagram::removeBox(Box *b)
{
  _p->removeBox(b);
}

void Diagram::loadAll()
{
  _p->loadAll();
}

void Diagram::unloadAll()
{
  _p->unloadAll();
}

void Diagram::loadFromDiagram(Diagram *d)
{
  _p->loadFromDiagram(d);
}

void Diagram::merge(Diagram *d)
{
  _p->merge(d);
}