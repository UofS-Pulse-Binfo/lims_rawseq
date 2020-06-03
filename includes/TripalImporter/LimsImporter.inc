<?php

/**
 * @see TripalImporter
 */
 class LimsImporter extends TripalImporter {

  /**
   * The name of this loader.  This name will be presented to the site
   * user.
   */
  public static $name = 'LIMS Importer';

  /**
   * The machine name for this loader. This name will be used to construct
   * the URL for the loader.
   */
  public static $machine_name = 'lims_importer';

  /**
   * A brief description for this loader.  This description will be
   * presented to the site user.
   */
  public static $description = 'Imports LIMS file into database.';

  /**
   * An array containing the extensions of allowed file types.
   */
  public static $file_types = ['tsv', 'txt'];

  public static $use_analysis = FALSE;
  /**
   * If the $use_analysis value is set above then this value indicates if the
   * analysis should be required.
   */
  public static $require_analysis = FALSE;

  /**
   * Provides information to the user about the file upload.  Typically this
   * may include a description of the file types allowed.
   */
  public static $upload_description = 'Germplasm file should be a tab separated file with the following colums:<ol>
      <li>Run Name: run name stored in dabase.</li>
      <li>File Name: full path and file name.</li>
      <li>md5sum: the checksum for the file.</li>
    </ol>';

  /**
   * Indicates the methods that the file uploader will support.
   */
  public static $methods = array(
    // Allow the user to upload a file to the server.
    'file_upload' => TRUE,
    // Allow the user to provide the path on the Tripal server for the file.
    'file_local' => TRUE,
    // Allow the user to provide a remote URL for the file.
    'file_remote' => TRUE,
  );

  public function form($form, &$form_state){

    $form['instructions'] = [
      '#weight' => -100,
      '#markup' => '
        <h2>Load LIMS file into database</h2>
        <p>Upload file must in TSV (tab-separated values) format. Please confirm file format and column order before upload.</p>
      ',
    ];


    return $form;
  }

  /**
   * @see TripalImporter::formValidate()
   */
  public function formValidate($form, &$form_state){
    //parent::formValidate($form, $form_state);
  }

  public function run(){
    $arguments = $this->arguments['run_args'];
    $file_path = $this->arguments['files'][0]['file_path'];

    $this->updatelimsinformation($file_path);
  }

  public function updatelimsinformation($file_path){
    $transaction = db_transaction();
    // print upload file name in Job LOGS
    $file_name_exp = explode('/', $file_path);
    $this->logMessage('Upload file name: !input_file ',['!input_file' => array_pop($file_name_exp)], TRIPAL_WARNING);

    if(!file_exists($file_path)){
	     var_dump(['Could not find file: ', $file_path]);
    }

    $fh = fopen($file_path, 'r');

    //read file, and explode each line
    while ($line_lims = fgets($fh)){
      // skip comments
      if (preg_match('/^#/', $line_germplasm)) {
        continue;
      }

      // skip empty lines
      if (preg_match('/^\s*$/', $line_germplasm)) {
        continue;
      }

      // For TEST of import files, files with end line of 'TEST_rollback' won't upload to DB
      if (preg_match('/^TEST_rollback/', $line_germplasm) ){
        $this->logMessage('Flag !rollback, is found in file, germplasm will not upload to database.',['!rollback' => 'TEST_rollback'], TRIPAL_WARNING);
        $transaction->rollback();
      }

      $lims_col = explode("\t", $line_lims);

      $one_lims_name = trim($lims_col[0]);

      $one_lims_path = trim($lims_col[1]);

      $one_lims_md5 = trim($lims_col[2]);

      $match = array(
        'run_name' => $one_lims_name,
      );

      $results = db_select('lims_seqrun', 'l')
        ->fields("l", array("run_id", "filename", "md5sum"))
        ->condition('run_name', $one_lims_name ,'=')
        ->execute()
        ->fetchAssoc();

      if ($results == FALSE){
        $this->logMessage('The import of lims to table:lims_seqrun, !one_lims failed.',['!one_lims' => $one_lims_name], TRIPAL_WARNING);
      }
      else{
        //check if file path and md5sum exist, insert if not
        $identified_lims_id = $results['run_id'];
        // update file path/md5sum

        $result_update = db_update('lims_seqrun')
          ->fields(array(
            'filename' => $one_lims_path,
            'md5sum'=> $one_lims_md5
          ))
          ->condition('run_id', $identified_lims_id, '=')
          ->execute();

        if ($result_update == 0){
          $this->logMessage('The file path and md5sum update for lims, !one_l failed.',['!one_l' => $one_lims_name], TRIPAL_WARNING);
        }

      }

    }
  }
}
