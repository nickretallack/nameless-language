let f =
    (definition: Definition.t, language: LanguageName.t, isInputs: bool)
    : list(DisplayNib.t) =>
  Belt.List.map(
    isInputs
      ? definition.display.inputOrdering : definition.display.outputOrdering,
    nibID => {
      let documentation = definition.documentation;
      let nibs = isInputs ? documentation.inputs : documentation.outputs;
      let translatable = Belt.Map.String.getExn(nibs, nibID);
      let vettable =
        Belt.Map.String.getExn(translatable.translations, language);
      DisplayNib.{nib: NibConnection(nibID), name: vettable.text};
    },
  );
