module Index = {
  let className = {
    open Css
    list{marginRight(#px(10))} |> style
  }
  @react.component
  let make = () => <>
    {list{"card", "pager", "goo", "numbers", "pull", "text"}
    |> List.map(url =>
      <div key=url className> <a href={"/" ++ url}> {url->React.string} </a> </div>
    )
    |> Array.of_list
    |> React.array}
  </>
}

@react.component
let make = () => {
  let url = RescriptReactRouter.useUrl()
  Js.log2(url, url.path)

  switch url.path {
  | list{"card"} => <Card />
  | list{"pager"} => <ViewPager />
  | list{"numbers"} => <Numbers />
  | list{"goo"} => <Goo />
  | list{"pull"} => <Pull />
  | list{"text"} => <Text />
  | _ => <Index />
  }
}
