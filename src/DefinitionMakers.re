let v = [|
  ("Function", () => GraphMake.f(~outputs=[|(RandomIDMake.f(), "")|], ())),
  (
    "Constant",
    () =>
      DefinitionMake.f(
        ~implementation=ConstantImplementation(TextValue("")),
        (),
      ),
  ),
  (
    "Interface",
    () => {
      let outputID = RandomIDMake.f();
      DefinitionMake.f(
        ~outputs=[|(outputID, "")|],
        ~implementation=
          InterfaceImplementation({
            input: Belt.Map.String.empty,
            output:
              Belt.Map.String.fromArray([|
                (outputID, ValueType.PrimitiveValueType(TextType)),
              |]),
          }),
        (),
      );
    },
  ),
  (
    "Record",
    () => {
      let inputID1 = RandomIDMake.f();
      let inputID2 = RandomIDMake.f();
      DefinitionMake.f(
        ~inputs=[|(inputID1, ""), (inputID2, "")|],
        ~implementation=
          RecordTypeImplementation(
            Belt.Map.String.fromArray([|
              (inputID1, ValueType.PrimitiveValueType(TextType)),
              (inputID2, ValueType.PrimitiveValueType(TextType)),
            |]),
          ),
        (),
      );
    },
  ),
|];
