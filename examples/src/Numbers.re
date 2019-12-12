
[%bs.raw {|require('./Numbers.css')|}];

type prop = {
  opacity: string,
  transform: string,
};
module TransitionHooks =
  Spring.MakeTransition({
    type t = prop;
    type item = int;
  });

let pages = [|
  (style, key) =>
    <Spring.Div
      key
      style={ReactDOMRe.Style.make(
        ~opacity=style.opacity,
        ~transform=style.transform,
        ~backgroundColor="lightpink",
        (),
      )}>
      "1"->React.string
    </Spring.Div>,
  (style, key) =>
    <Spring.Div
      key
      style={ReactDOMRe.Style.make(
        ~opacity=style.opacity,
        ~transform=style.transform,
        ~backgroundColor="lightblue",
        (),
      )}>
      "2"->React.string
    </Spring.Div>,
  (style, key) =>
    <Spring.Div
      key
      style={ReactDOMRe.Style.make(
        ~opacity=style.opacity,
        ~transform=style.transform,
        ~backgroundColor="lightgreen",
        (),
      )}>
      "3"->React.string
    </Spring.Div>,
|];
[@react.component]
let make = () => {
  let (index, setIndex) = React.useState(_ => 0);
  let transitions =
    TransitionHooks.use(
      index,
      TransitionHooks.config(
        ~from={opacity: "0", transform: "translate3d(100%,0,0)"},
        ~enter={opacity: "1", transform: "translate3d(0%,0,0)"},
        ~leave={opacity: "0", transform: "translate3d(-50%,0,0)"},
        (),
      ),
    );
  let onClick = React.useCallback0(_ => setIndex(state => (state + 1) mod 3));
  <div className="numbers">
    <div className="simple-trans-main" onClick>
      {Array.map(
         ({TransitionHooks.item, props, key}) => {
           let page = Array.unsafe_get(pages, item);
           // <Page key={key} style={props} />
           page(props, key);
         },
         transitions,
       )
       |> React.array}
    </div>
  </div>;
};