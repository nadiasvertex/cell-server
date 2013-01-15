"""
A table is a collection of data engine column objects. The actual compute
load is performed in native code. The collection of data columns is handled
here in Python because the columns of a table tend to be of different
types, and dealing with that kind of situation in native code leads to
a lot of nonsense.
"""

import data_engine

class Table:
   def __init__(self, column_types):
      """
      :synopsis: Initializes a table object.

      :param column_types: A tuple of column type specifiers used to
      construct the list of tables.
      """
