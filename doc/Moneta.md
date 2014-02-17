# Moneta

## Objective

To write code like this:

    moneta::context context(soci_context);
    context.read<Person>
    // etc etc etc

## The Layers

### Level 1: Entity Hierarchies

- Address
  - ID
  - Number
  - Street

- Person
  - ID
  - Name
  - Address

### Level N: Backends

- Generic Object Multiplicity
  - Used by backends, like SQL, for cardinality.
