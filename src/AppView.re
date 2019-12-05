[@react.component]
let make = () => {
  let connectionSide =
    ConnectionSide.{node: GraphConnection, nib: ValueConnection};
  let explicitConnectionSide =
    ExplicitConnectionSide.{connectionSide, isSource: false};
  <div>
    {React.string(ConnectionSideToString.f(connectionSide))}
    {React.string(Json.stringify(ConnectionSideToJson.f(connectionSide)))}
    {React.string(ExplicitConnectionSideKey.f(explicitConnectionSide))}
  </div>;
};
