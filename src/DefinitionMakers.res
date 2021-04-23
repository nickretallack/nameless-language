let v = [
  (
    "Function",
    (languageName: LanguageName.t) =>
      GraphMake.f(languageName, ~outputs=[(RandomIDMake.f(), "")], ()),
  ),
  (
    "Constant",
    (languageName: LanguageName.t) =>
      DefinitionMake.f(languageName, ~implementation=ConstantImplementation(TextValue("")), ()),
  ),
  (
    "Interface",
    (languageName: LanguageName.t) => {
      let outputID = RandomIDMake.f()
      DefinitionMake.f(
        languageName,
        ~outputs=[(outputID, "")],
        ~implementation=InterfaceImplementation({
          input: Belt.Map.String.empty,
          output: Belt.Map.String.fromArray([(outputID, ValueType.PrimitiveValueType(TextType))]),
        }),
        (),
      )
    },
  ),
  (
    "Record",
    (languageName: LanguageName.t) => {
      let inputID1 = RandomIDMake.f()
      let inputID2 = RandomIDMake.f()
      DefinitionMake.f(
        languageName,
        ~inputs=[(inputID1, ""), (inputID2, "")],
        ~implementation=RecordTypeImplementation(
          Belt.Map.String.fromArray([
            (inputID1, ValueType.PrimitiveValueType(TextType)),
            (inputID2, ValueType.PrimitiveValueType(TextType)),
          ]),
        ),
        (),
      )
    },
  ),
  (
    "Label",
    (languageName: LanguageName.t) =>
      DefinitionMake.f(languageName, ~implementation=LabeledTypeImplementation(AnyType), ()),
  ),
  (
    "Symbol",
    (languageName: LanguageName.t) =>
      DefinitionMake.f(languageName, ~implementation=SymbolImplementation, ()),
  ),
  (
    "Union",
    (languageName: LanguageName.t) =>
      DefinitionMake.f(
        languageName,
        ~implementation=UnionTypeImplementation(TypeSet.fromArray([])),
        (),
      ),
  ),
]
