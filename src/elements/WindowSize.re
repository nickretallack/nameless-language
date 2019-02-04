[%%debugger.chrome];
open Helpers;

type action =
  | Resize(point);

[@bs.val] external innerWidth: float = "window.innerWidth";
[@bs.val] external innerHeight: float = "window.innerHeight";
[@bs.val]
external addEventListener: (string, Js.t({..}) => unit) => unit =
  "window.addEventListener";

let component = ReasonReact.reducerComponent("WindowSize");

let make = (~render, _children) => {
  ...component,
  initialState: () => {x: 1366.0, y: 768.0},
  reducer: (action, _state) =>
    switch (action) {
    | Resize(size) => ReasonReact.Update(size)
    },
  didMount: self => {
    self.send(Resize({x: innerWidth, y: innerHeight}));
    addEventListener("resize", _event =>
      self.send(Resize({x: innerWidth, y: innerHeight}))
    );
  },
  render: self => render(self.state),
};
