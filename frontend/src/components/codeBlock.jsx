import { stripIndent } from "common-tags";
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { vscDarkPlus } from 'react-syntax-highlighter/dist/esm/styles/prism';

export default function CodeBlock(props) {
  console.log(props.code);
  return (
    <div className="bg-gray-700 rounded-lg w-full max-w-screen-md mx-auto text-left">
      <p className="m-0 p-2  text-gray-400">{props.path}</p>
      <SyntaxHighlighter customStyle={{ margin: 0, borderBottomLeftRadius: 10, borderBottomRightRadius: 10 }} language="c" style={vscDarkPlus}>
        {stripIndent(props.code)}
      </SyntaxHighlighter>
    </div>
  )
}
