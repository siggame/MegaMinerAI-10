#-*-python-*-
from BaseAI import BaseAI
from GameObject import *

from itertools import product
from random import choice

class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "mnuck"

  @staticmethod
  def password():
    return "derpaderpaderp"
  
  
  def __init__(self, conn):
    BaseAI.__init__(self, conn)

  def init(self):
    pass

  def end(self):
    pass

  def run(self):
    world = generate_map(self)
    blue = [x for x in self.creatures if x.owner == self.playerID]

    for c in blue:
      c.done = False
    
    for c in blue:
      clear_nearby(c, world)
      
      while not c.done:
        dest = pick_move(c, world)
        smart_move(c, dest, world)
        if c.movementLeft == 0:
          c.done = True
          break
        clear_nearby(c, world)

    view_world(world, self.getMapWidth(), self.getMapHeight())
    return 1

    
def clear_nearby(c, world):
  neighbors = get_neighbors(c, world)
  for n in neighbors:
    if n['edible']:
      smart_eat(c, n, world)
      c.done = True
      break
  

def pick_move(c, world):
  result = nearest_plant(c, world)
  if result is None:
    result = nearest_prey(c, world)
  return result


def nearest(c, world, start, is_goal):
  [world[x].update({'start':None}) for x in world]  
  frontier = [x for x in get_neighbors(c, world) if start(x)]
  [f.update({'start': f}) for f in frontier]
  while frontier:
    current = frontier.pop(0)
    if is_goal(current):
      return current['start']
    else:
      neighbors = [x for x in get_neighbors(current, world) 
                   if x['start'] is None]
      [n.update({'start': current['start']}) for n in neighbors]
      frontier.extend(neighbors)
  return None

def nearest_plant(c, world):
  return nearest(c, world,
                 lambda x: x['creature'] is None,
                 lambda x: x['plant'] is not None)

def nearest_prey(c, world):
  return nearest(c, world,
                 lambda x: x['plant'] is None,
                 lambda x: x['creature'] is not None and x['edible'])


def get_neighbors(thing, world):
  try:
    x, y = thing.x, thing.y
  except:
    x, y = thing['coords'][0], thing['coords'][1]
  potential = [(x + 1, y), (x - 1, y),
               (x, y + 1), (x, y - 1)]
  return [world[coords] for coords in potential if coords in world]


def smart_eat(c, dest, world):
  if dest['plant'] is not None:
    if dest['plant'].size == 0:
      return
  if not c.canEat:
    return
  c.eat(dest['coords'][0], dest['coords'][1])
  if dest['plant'] is None:
    target = 'creature'
    print c.id, 'eating', dest['coords'], target, dest[target].id, dest[target].x, dest[target].y
    
  else:
    target = 'plant'
    if world[dest['coords']]['plant'].size < 1:
      world[dest['coords']].update({'edible': False,
                                    'pathable': True})
    print c.id, 'eating', dest['coords'], target, dest[target].id, dest[target].x, dest[target].y, dest[target].size


def smart_move(c, dest, world):
  if c.movementLeft == 0:
    c.done = True
    return
  dest = dest['coords']
  source = (c.x, c.y)
  if source == dest:
    c.done = True
    return
  if not world[dest]['pathable']:
    c.done = True
    return
  world[dest].update({'creature': c,
                      'edible': False,
                      'pathable': False})
  world[source].update({'creature': None,
                        'edible': False,
                        'pathable': True})
  c.move(dest[0], dest[1])
  #print c.id, 'moving', source, '->', dest  

    
def generate_map(o):
  world = dict()
  for coords in product(xrange(o.getMapWidth()), xrange(o.getMapHeight())):
    world[coords] = {'pathable': True, 
                     'edible': False, 
                     'coords': coords,
                     'creature': None,
                     'plant': None}

  for p in o.plants:
    coords = (p.x, p.y)
    world[coords].update({'plant': p})
    if p.size > 0:
      world[coords].update({'edible': True, 'pathable': False})

  for c in o.creatures:
    coords = (c.x, c.y)
    world[coords].update({'creature': c, 'pathable': False})
    if c.owner != o.getPlayerID():
      world[coords].update({'edible': True})

  return world


def view_world(w, max_x, max_y):
  for j in xrange(max_y):
    row = ""
    for i in xrange(max_x):
      if w[(i,j)]['creature'] is not None:
        if w[(i,j)]['edible']:
          thing = 'o'
        else:
          thing = 'X'
      elif w[(i,j)]['plant'] is not None:
        thing = '.'
      else:
        thing = ' '
      row += thing
    print row
  print "*************************************"
