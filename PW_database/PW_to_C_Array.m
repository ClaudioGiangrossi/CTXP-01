function C_Array = PW_to_C_Array(data, name)

    C_Array = round((data' ./ max(data)) * 255);
    
    fileID = fopen('C_Array.txt','w');
    fprintf(fileID, 'profilo_t %s = {\n', name);
    fprintf(fileID, '\t"%s",\n', name);
    fprintf(fileID, '\t%d,\n', size(C_Array,2));
    fprintf(fileID, '\tNULL, \n');
    fprintf(fileID, '\tNULL, \n');
    fprintf(fileID, '\t{');
    fprintf(fileID, '%d, ', C_Array);
    fprintf(fileID, '}\n');
    fprintf(fileID, '};');