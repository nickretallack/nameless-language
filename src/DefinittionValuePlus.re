let v =
  DefinitionMake.f(
    ~name="+",
    ~description="Add two numbers, resulting in a number.",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "+",
        interface: {
          input:
            Belt.Map.String.fromArray([|
              ("left", ValueType.PrimitiveValueType(NumberType)),
              ("right", ValueType.PrimitiveValueType(NumberType)),
            |]),
          output:
            Belt.Map.String.fromArray([|
              ("result", ValueType.PrimitiveValueType(NumberType)),
            |]),
        },
      }),
    (),
  );
