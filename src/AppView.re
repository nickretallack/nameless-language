[@react.component]
let make = () => {
  <div>
    {React.string("hello")}
    {React.string(
       Json.stringify(
         EncodeConnectionNode.f(ConnectionNode.GraphConnection),
       ),
     )}
  </div>;
};
