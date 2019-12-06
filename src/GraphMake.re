let f =
    (
      ~name="",
      ~description="",
      ~inputs=[||],
      ~outputs=[||],
      ~nodes=[||],
      ~connections=[||],
      _unit,
    ) =>
  DefinitionMake.f(
    ~name,
    ~description,
    ~inputs,
    ~outputs,
    ~implementation=
      GraphImplementation({
        interface: {
          input:
            Belt.Map.String.fromArray(
              Belt.Array.map(inputs, ((id, _name)) =>
                (id, ValueType.AnyType)
              ),
            ),
          output:
            Belt.Map.String.fromArray(
              Belt.Array.map(outputs, ((id, _name)) =>
                (id, ValueType.AnyType)
              ),
            ),
        },
        nodes: Belt.Map.String.fromArray(nodes),
        connections:
          Belt.Map.fromArray(
            connections,
            ~id=(module ConnectionSideComparable.C),
          ),
      }),
    (),
  );
