@react.component
let make = (
  ~ordering: array<NibID.t>,
  ~nibDocumentations: Belt.Map.String.t<Translatable.t>,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~execution: Execution.t,
  ~nibToSource: NibID.t => ConnectionSide.t,
  ~scope: Scope.t,
) =>
  <div>
    {React.array(
      Belt.Array.mapWithIndex(ordering, (_index, nibID) =>
        switch SourceResolveValue.f(scope, nibToSource(nibID), execution, definitions) {
        | None => React.null
        | Some(value) =>
          let name = TranslatableGetText.f(
            Belt.Map.String.getExn(nibDocumentations, nibID),
            languageName,
          )
          let value = ValueDisplay.f(value, execution, definitions, languageName)
          <div key={nibID}>
            {React.string(
              if name == "" {
                value
              } else {
                `${name}: ${value}`
              },
            )}
          </div>
        }
      ),
    )}
  </div>
