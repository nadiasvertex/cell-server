# Introduction #

A row store database may process multi-column predicates at the same time for each row. This makes sense, especially if the row has no index, because the row-store has all of the columns available. However, cell is a column-store. Therefore, it makes sense to process all predicates for a specific column at once.

# How Queries are Re-Written #

## Simple Multi Column Predicates ##

```
SELECT name 
FROM volunteers
WHERE state='CO' AND sex='F';
```

This simple query will select all female volunteers currently living in Colorado. The query is rewritten in a simple way:

```
filter<string, filter_op::EQ> p1("CO");
filter<string, filter_op::EQ> p2("F");
```

The filters are then run one at a time on each column:

```
auto state_matches = col_state.find(p1);
auto sex_matches = col_sex.find(p2);
std::set<column_id> final_matches;
std::set_intersection(state_matches.begin(), state_matches.end(),
                      sex_matches.begin(), sex_matches.end(),
                      final_matches());
```

## Simple Single Column Predicates ##

```
SELECT name 
FROM volunteers
WHERE age >= 20 AND age <= 40;
```

This simple query will select all volunteers between the ages of 30 and 40.

```
auto p = binary_predicate<int, true>(
           filter<int, filter_op::GE>(20),
           filter<int, filter_op::LE>(40)
); 
auto final_matches = col_age.find(p);
```

This is much faster than executing two filters over the same data set.

## Complex Multi Column Predicates ##

```
SELECT name 
FROM volunteers
WHERE (age >= 20 AND age <= 40 AND state = 'CO') OR
      (age >= 25 AND age <= 30 AND state = 'WY');
```

In this case we want to execute a more complex common set. We rewrite it into two different predicates:

```
SELECT name 
FROM volunteers
WHERE ((age >= 20 AND age <= 40) OR (age >= 25 AND age <= 30)) AND 
      (state = 'CO' OR state = 'WY');
```

The original expression tree looks like that:

![http://cell-server.googlecode.com/hg/doc/query_rewrite.dot.png](http://cell-server.googlecode.com/hg/doc/query_rewrite.dot.png)

Once rewritten, the expression tree looks like this:

![http://cell-server.googlecode.com/hg/doc/query_rewritten.dot.png](http://cell-server.googlecode.com/hg/doc/query_rewritten.dot.png)

The final code generated would look more or less like this:

```
auto p1 = binary_predicate<int, false>(
            binary_predicate<int, true>(
              filter<int, filter_op::GE>(20),
              filter<int, filter_op::LE>(40)
            ),
            binary_predicate<int, true>(
              filter<int, filter_op::GE>(25),
              filter<int, filter_op::LE>(30)
            )
);
auto p2 =  binary_predicate<string, false>(
              filter<string, filter_op::EQ>("CO"),
              filter<string, filter_op::EQ>("WY")
);
auto age_matches = col_age.find(p1);
auto state_matches = col_state.find(p2);
std::set<column_id> final_matches;
std::set_intersection(state_matches.begin(), state_matches.end(),
                      age_matches.begin(), age_matches.end(),
                      final_matches());
```

The algorithm for re-write is very simple. As you go, left-first, depth-first down the tree, check to see if you are at an AND or OR operation. If so, check and see if both sides reference the same column. If they do, good, go up one level of the tree.

If they don't:

1. If you are on the left branch of the current node's parent: swap the current node's right side with the nearest boolean operation's parent's right side. Then swap the operation of each node. For example if the current node was "AND" and the parent operation was "OR", then we would make the current operation "OR" and the parent "AND".

2. If you are on the right side of the current node's parent: we have to move the node higher. Make our left node our parent's right node. Then take the current node and make us our grandparent's left. Make our parent our left.