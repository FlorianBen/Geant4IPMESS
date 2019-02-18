import sys
sys.path.append('/usr/lib/freecad/lib') # Change with respect to your installation
import FreeCAD, Mesh, Import

import json

def main():
  """
  Convert all parts in a STEP file to many STL files plus TXT offset files.
  """
  Import.open(sys.argv[1])
  doc = FreeCAD.activeDocument()
  i = 0
  for obj in doc.Objects:
    obj_export = []
    obj_export.append(obj)
    a = obj.Name
    b = obj.Label
    filename = a + '_' + "".join( x for x in b if (x.isalnum() or x in "._- ")) + str(i).zfill(4)
    f = open(sys.argv[2] + filename + '.txt', 'w')
    f.write(a + ' -> ' + b + '\n')
    f.write(str(obj.Placement)+'\n')
    f.write(str(obj.Shape.Placement)+'\n')
    f.write(str(obj.Shape.BoundBox)+'\n')
    #f.write(str(obj.Shape.CenterOfMass)+'\n')
    f.close()
    Mesh.export(obj_export, sys.argv[2] + filename + ".stl")
    i = i +1

if __name__=='__main__':
   main()