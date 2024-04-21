import React, { useState, useEffect } from 'react';
import { FileUpload } from 'primereact/fileupload';
import Verdict from "./components/verdict";

export default function App() {
  const [ uploaded, setUploaded ] = useState(false);
  const [ loading, setLoading ] = useState(false);

  useEffect(() => {
    if(!loading) return;

    setTimeout(() => {
      setUploaded(true);
      setLoading(false);
    }, 5000);
  }, [loading])
  return (
    loading ? <p className="m-0">Loading...</p> :
    uploaded
    ? (
      <div className='w-full h-full w-max-screen h-max-screen bg-gray-50 dark:bg-dark-800'>
          <Verdict />
      </div>
    ) 
    : (
      <div className="card">
        <FileUpload 
            name="file" 
            url={'http://localhost:5000/upload'} 
            onSelect={() => console.log("Selected")}
            onUpload={() => setLoading(true)}
            multiple 
            accept=".zip"
            maxFileSize={1000000} 
            emptyTemplate={
                <p className="m-0">Drag and drop .zip file with code files</p>
            } 
        />
    </div>
    )
  )
}

