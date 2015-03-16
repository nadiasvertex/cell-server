# Introduction #

The processing of a query is somewhat more complex than in typical database systems. This complexity is necessary in order to simplify the entire system by allowing application code to integrate directly with the query processing engine.

The following description provides a rough overview of how this processing is intended to function.

Assume the query is SQL-based, and references this table:

```
CREATE TABLE table1 (id bigserial, col1 int, col2 int);
```

A query like this:

```
SELECT col1 FROM table1 WHERE col2 > 10 AND col2 < 100;
```

First gets pre-processed into this:

```
SELECT col1 FROM table1 WHERE col2 > :p1 AND col2 < :p2;
```

It then gets processed into a C# assembly by converting the SQL into this C# code:

```

namespace query_cache 
{

[ProtoContract]
public class query_row_0123456789
{
	[ProtoMember(1)]
	public int col1 { get; set; }
}

public class query_0123456789
{
  int p1, p2;
  data_engine de;
  
  class query_0123456789(data_engine _de, int _p1, int _p2)
  {
	de = _de;
	p1 = _p1;
	p2 = _p2;
  }

  public bool filter(long col2_column_id)
  {
	var col2_index = 2;
	int col2;
	
	de.get_column(col2_index).get_value(col2_column_id, ref col2);
	
	return col2 > p1 && col2 < p2;
  }
  
  public int select_expr1(long col1_column_id)
  {
	return de.get_column(column_col1_index;
  }
  
  public void serialize_row(long col1_column_id, Stream s)
  {
	Serializer.Serialize<query_row_0123456789>(
		new query_row_0123456789(select_expr1(col1_column_id))
	);
  }  
  
  public static query_0123456789 create(data_engine de, int p1, int p2)
	{
	 return query_0123456789(de, p1, p2);
	}
}

}

```

Once this assembly exists, it is distributed to all nodes in the cluster. Then the C++ part of the data engine does something like this:

```
int arg1 = 10;
int arg2 = 100;

auto* assembly = mono->load("query_0123456789.dll")
auto* factory = mono->find_function(assembly, "query_cache", "query_0123456789", "create");
auto* qo = mono->apply(factory, {data_engine_object, &arg1, &arg2});
auto* filter = mono->find_member_function(assembly, qo, "filter");
auto* serialize = mono->find_member_function(assembly, qo, "serialize");

for(const auto& row : rows)
{
   auto col2_column_id = row->second[col2_index];
   auto* result = mono->apply(filter, {&col2_column_id});
   if (mono->is_true(result)) 
   {
      auto col1_column_id = row->second[col1_index];
      mono->apply(serialize, {stream_object, &col1_column_id});
   }
}

```