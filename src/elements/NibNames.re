open Helpers;

type textRole =
  | Source
  | Sink
  | Title;

type line = {
  role: textRole,
  text: string,
};
open Definition;
let component = ReasonReact.statelessComponent("NibNames");
let make =
    (
      ~position: point,
      ~height: float,
      ~title=?,
      ~sources=[],
      ~sinks=[],
      ~nodeWidth: float,
      ~textHeight: float,
      _children,
    ) => {
  ...component,
  render: _self => {
    let sidePadding = 10.0;
    let lines =
      Belt.List.concatMany([|
        switch (title) {
        | None => []
        | Some(title) => [{role: Title, text: title}]
        },
      |]);

    let renderText = (text, index, textRole) =>
      <text
        textAnchor={
          switch (textRole) {
          | Source => "start"
          | Sink => "end"
          | Title => "middle"
          }
        }
        alignmentBaseline="central"
        x={string_of_float(
          position.x
          +. (
            switch (textRole) {
            | Source => sidePadding
            | Sink => nodeWidth -. sidePadding
            | Title => nodeWidth /. 2.0
            }
          ),
        )}
        y={string_of_float(textHeight *. (float_of_int(index) +. 0.5))}>
        {ReasonReact.string(text)}
      </text>;

    let renderTexts = (nibs: list(displayNib), isSource: bool) =>
      ReasonReact.array(
        Belt.List.toArray(
          Belt.List.mapWithIndex(nibs, (index, display: displayNib) =>
            renderText(
              display.name,
              index + (title != None ? 1 : 0),
              isSource ? Source : Sink,
            )
          ),
        ),
      );

    let renderedTitle =
      switch (title) {
      | None => ReasonReact.null
      | Some(text) => renderText(text, 0, Title)
      };

    <>
      <rect
        x={string_of_float(position.x)}
        y={string_of_float(position.y)}
        width={string_of_float(nodeWidth)}
        height={string_of_float(height *. textHeight)}
        fill="black"
        fillOpacity="0.1"
      />
      renderedTitle
      {renderTexts(sources, true)}
      {renderTexts(sinks, false)}
    </>;
  },
};
