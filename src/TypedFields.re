open Definition;

let component = ReasonReact.statelessComponent("TypedFields");

let make =
    (
      ~typedFields: typedFields,
      ~nibDocumentations: Belt.Map.String.t(translatable),
      ~isInput: bool,
      ~ordering: list(nibID),
      ~emit,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div>
      {ReasonReact.array(
         Belt.List.toArray(
           Belt.List.map(ordering, nibID =>
             <TypedField
               valueType={Belt.Map.String.getExn(typedFields, nibID)}
               name={Belt.Map.String.getExn(nibDocumentations, nibID)}
               emit
               isInput
               nibID
             />
           ),
         ),
       )}
    </div>;
  },
};
