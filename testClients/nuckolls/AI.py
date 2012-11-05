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

  def init(self):
    pass

  def end(self):
    pass

  def run(self):
    world = generate_map(self)
    blue = [x for x in self.creatures if x.owner == self.playerID]
    
    for c in blue:
      neighbors = get_neighbors(c, world)
      # eat nearby things
      for n in neighbors:
        if n['edible']:
          smart_eat(c, n, world)
          break
      
      dest = pick_move(c, world)
      smart_move(c, dest, world)
      
    view_world(world, self.getMapWidth(), self.getMapHeight())

    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)


def pick_move(c, world):
  return nearest_plant(c, world)
  #neighbors = get_neighbors(c, world)
  #movable = [x for x in neighbors if x['pathable']]
  #return choice(movable)
    

class Loc(object):
  def __init__(self, x, y):
    self.x = x
    self.y = y
    self.coords = (x, y)
  

def nearest_plant(c, world):
  result = world[(c.x, c.y)]
  for x in world:
    world[x]['ancestor'] = None

  frontier = get_neighbors(c, world)
  frontier = [x for x in frontier if x['ancestor'] is None]
  frontier = [x for x in frontier if x['creature'] is None]
  for f in frontier:
    f['ancestor'] = f
  while frontier:
    current = frontier.pop(0)
    if current['plant'] is None:
      neighbors = get_neighbors(current, world)
      neighbors = [x for x in neighbors if x['ancestor'] is None]
      neighbors = [x for x in neighbors if x['creature'] is None]
      for n in neighbors:
        n['ancestor'] = current['ancestor']
      frontier.extend(neighbors)
    else:
      return current['ancestor']
  return result


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
  dest = dest['coords']
  source = (c.x, c.y)
  if source == dest:
    return
  if not world[dest]['pathable']:
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
