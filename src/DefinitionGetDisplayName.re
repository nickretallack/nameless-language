let t = (definition: Definition.t, language: LanguageName.t) => {
  let text = TranslatableGetText.f(definition.documentation.name, language);
  if (text != "") {
    text;
  } else {
    switch (definition.implementation) {
    | ConstantImplementation(value) => PrimitiveValueDisplay.f(value)
    | _ =>
      "(nameless "
      ++ ImplementationGetName.f(definition.implementation)
      ++ ")"
    };
  };
};
