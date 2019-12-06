let v =
  DefinitionMake.f(
    ~name="2D Point",
    ~description="X and Y coordinates",
    ~inputs=[|("x", "X"), ("y", "Y")|],
    ~implementation=
      RecordTypeImplementation(
        Belt.Map.String.fromArray([|
          ("x", ValueType.PrimitiveValueType(NumberType)),
          ("y", ValueType.PrimitiveValueType(NumberType)),
        |]),
      ),
    (),
  );
