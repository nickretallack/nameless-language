[%%debugger.chrome];
open Definition;

let component = ReasonReact.statelessComponent("TypedFields");

let make =
    (
      ~definitions: definitions,
      ~typedFields: typedFields,
      ~nibDocumentations: Belt.Map.String.t(translatable),
      ~isInput: bool,
      ~ordering: list(nibID),
      ~emit,
      _children,
    ) => {
  ...component,
  render: _self => {
    <table>
      <tbody>
        {ReasonReact.array(
           Belt.List.toArray(
             Belt.List.mapWithIndex(ordering, (index, nibID) =>
               <TypedField
                 key=nibID
                 valueType={Belt.Map.String.getExn(typedFields, nibID)}
                 name={Belt.Map.String.getExn(nibDocumentations, nibID)}
                 index
                 count={Belt.List.length(ordering)}
                 definitions
                 emit
                 isInput
                 nibID
               />
             ),
           ),
         )}
      </tbody>
    </table>;
  },
};
