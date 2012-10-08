#!/usr/bin/env python
from argparse import ArgumentParser
from os import mkdir, symlink, listdir, chdir
from os.path import isdir, basename, splitext
from shutil import copy


if __name__ == '__main__':
  parser = ArgumentParser(description='Sets up new client directory')

  parser.add_argument('language', metavar='LANG', type=str, help='Language your client will be. (e.g. python, cpp, java, csharp, etc')
  parser.add_argument('name', metavar='NAME', type=str, help='Folder your client will be installed to')

  args = parser.parse_args()
  if isdir(args.name):
    raise Exception('Client With That Name Already Exists In This Directory')
  if not isdir('../client/{0}'.format(args.language)):
    raise Exception('That Language Implementation Does Not Exist!  Please choose a valid language or implement a new one.')

  mkdir(args.name)
  chdir(args.name)

  for f in listdir('../../client/{0}/'.format(args.language)):
    operation = symlink
    if splitext(basename(f))[0].lower() in ['ai', 'makefile', 'run', 'main']:
      operation = copy
    operation('../../client/{0}/{1}'.format(args.language, f), './{0}'.format(f))



