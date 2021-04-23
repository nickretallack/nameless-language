[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~languageName: LanguageName.t,
      ~typedFields: RecordType.t,
      ~nibDocumentations: Belt.Map.String.t(Translatable.t),
      ~isInput: bool,
      ~ordering: list(NibID.t),
      ~emit,
    ) => {
  <table>
    <tbody>
      {React.array(
         Belt.List.toArray(
           Belt.List.mapWithIndex(ordering, (index, nibID) =>
             <TypedFieldView
               key=nibID
               valueType={Belt.Map.String.getExn(typedFields, nibID)}
               name={Belt.Map.String.getExn(nibDocumentations, nibID)}
               index
               count={Belt.List.length(ordering)}
               definitions
               languageName
               emit
               isInput
               nibID
             />
           ),
         ),
       )}
    </tbody>
  </table>;
};
