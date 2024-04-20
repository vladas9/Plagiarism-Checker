import React from 'react';
import { FileUpload } from 'primereact/fileupload';
import Verdict from "./components/verdict";

export default function App() {
  return (
    //<div className='w-full h-full w-max-screen h-max-screen bg-gray-50 dark:bg-dark-800'>
    <Verdict />
    //</div>
    // <div className="card">
    //     <FileUpload 
    //         name="demo[]" 
    //         url={'http://192.168.188.76:8000'} 
    //         onSelect={console.log}
    //         onUpload={console.log}
    //         multiple 
    //         accept=".c,.txt"
    //         maxFileSize={1000000} 
    //         emptyTemplate={
    //             <p className="m-0">Drag and drop files to here to upload.</p>
    //         } 
    //     />
    // </div>
  )
}

