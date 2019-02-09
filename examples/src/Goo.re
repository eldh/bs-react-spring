[@bs.config {jsx: 3}];
[%bs.raw {|require('./Goo.css')|}];

let component = ReasonReact.statelessComponent(__MODULE__);
let fast =
  Spring.config(~tension=1200., ~friction=40., ~mass=Obj.magic(Js.null));
let slow = Spring.config(~tension=200., ~friction=50., ~mass=10.);
let trans = (x, y) => {j|translate3d($(x)px,$(y)px,0) translate3d(-50%,-50%,0)|j};

module TrailHook =
  Spring.MakeTrail({
    type t = (float, float);
    let number = 3;
    type interpolate = (float, float) => string;
  });

[@react.component]
let make = () => {
  let (trail, set) =
    TrailHook.use(~config=i => i === 0 ? fast : slow, () => (0, 0));

  <>
    <svg
      style={ReactDOMRe.Style.make(
        ~position="absolute",
        ~width="0",
        ~height="0",
        (),
      )}>
      <filter id="goo">
        <feGaussianBlur result="blur" stdDeviation="30" />
        <feColorMatrix values="1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 30 -7" />
      </filter>
    </svg>
    <div
      className="hooks-main"
      onMouseMove={e => {
        let vals = (
          ReactEvent.Mouse.clientX(e)->float_of_int,
          ReactEvent.Mouse.clientY(e)->float_of_int,
        );
        set(vals);
      }}>
      {trail
       |> Array.mapi((index, props) =>
            <Spring.Div
              key={index->string_of_int}
              style={ReactDOMRe.Style.make(
                ~transform=props->TrailHook.interpolate(trans),
                (),
              )}
            />
          )
       |> ReasonReact.array}
    </div>
  </>;
};