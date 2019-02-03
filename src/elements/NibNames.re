open Helpers;
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

    let renderText = (text, index, isSource) =>
      <text
        textAnchor={isSource ? "start" : "end"}
        alignmentBaseline="central"
        x={string_of_float(
          position.x +. (isSource ? sidePadding : nodeWidth -. sidePadding),
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
              isSource,
            )
          ),
        ),
      );

    <>
      <rect
        x={string_of_float(position.x)}
        y={string_of_float(position.y)}
        width={string_of_float(nodeWidth)}
        height={string_of_float(height *. textHeight)}
        fill="black"
        fillOpacity="0.1"
      />
      {renderTexts(sources, true)}
      {renderTexts(sinks, false)}
    </>;
  },
};
