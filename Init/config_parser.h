#include <stdlib.h>
#include <stdio.h>

#define FILENAME "../config_file.txt"

int set_var(char *var_name, char *var_value, struct _shared_memory* shared_memory)
{
    if (!strcmp(var_name, "BEGIN"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->BEGIN = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->BEGIN = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_critical_event"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->local_critical_event = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->local_critical_event = FALSE;
            return 1;
        }
        else
            return 0;

    }
    else if (!strcmp(var_name, "local_dying_gasp"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->local_dying_gasp = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->local_dying_gasp = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_lost_link_timer_done"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->local_lost_link_timer_done = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->local_lost_link_timer_done = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_satisfied"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->local_satisfied = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->local_satisfied = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_stable"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->local_stable = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->local_stable = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "remote_stable"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->remote_stable = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->remote_stable = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "remote_state_valid"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->remote_state_valid = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->remote_state_valid = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_unidirectional"))
    {
        if (!strcmp(var_value, "TRUE"))
        {
            shared_memory->local_unidirectional = TRUE;
            return 1;
        }
        else if (!strcmp(var_value, "FALSE"))
        {
            shared_memory->local_unidirectional = FALSE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "reception_status"))
    {
        /*
            Values Not Known Here
        */

        if (!strcmp(var_value, "receiveOK"))
        {
            shared_memory->reception_status = receiveOK;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_link_status"))
    {
        if (!strcmp(var_value, "FAIL"))
        {
            shared_memory->local_link_status = FAIL;
            return 1;
        }
        else if (!strcmp(var_value, "OK"))
        {
            shared_memory->local_link_status = OK;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_oam_enable"))
    {
        if (!strcmp(var_value, "DISABLE"))
        {
            shared_memory->local_oam_enable = DISABLE;
            return 1;
        }
        else if (!strcmp(var_value, "ENABLE"))
        {
            shared_memory->local_oam_enable = ENABLE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_oam_mode"))
    {
        if (!strcmp(var_value, "PASSIVE"))
        {
            shared_memory->local_oam_mode = PASSIVE;
            return 1;
        }
        else if (!strcmp(var_value, "ACTIVE"))
        {
            shared_memory->local_oam_mode = ACTIVE;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_par_action"))
    {
        if (!strcmp(var_value, "DISCARD"))
        {
            shared_memory->local_par_action = par_DISCARD;
            return 1;
        }
        else if (!strcmp(var_value, "LB"))
        {
            shared_memory->local_par_action = par_LB;
            return 1;
        }
        else if (!strcmp(var_value, "FWD"))
        {
            shared_memory->local_par_action = par_FWD;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_mux_action"))
    {
        if (!strcmp(var_value, "DISCARD"))
        {
            shared_memory->local_mux_action = mux_DISCARD;
            return 1;
        }
        else if (!strcmp(var_value, "FWD"))
        {
            shared_memory->local_mux_action = mux_FWD;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "local_pdu"))
    {
        if (!strcmp(var_value, "RX_INFO"))
        {
            shared_memory->local_pdu = RX_INFO;
            return 1;
        }
        else if (!strcmp(var_value, "LF_INFO"))
        {
            shared_memory->local_pdu = LF_INFO;
            return 1;
        }
        else if (!strcmp(var_value, "INFO"))
        {
            shared_memory->local_pdu = INFO;
            return 1;
        }
        else if (!strcmp(var_value, "ANY"))
        {
            shared_memory->local_pdu = ANY;
            return 1;
        }
        else
            return 0;
    }
    else if (!strcmp(var_name, "pdu_req"))
    {
        if (!strcmp(var_value, "NONE"))
        {
            shared_memory->pdu_req = NONE;
            return 1;
        }
        else if (!strcmp(var_value, "CRITICAL"))
        {
            shared_memory->pdu_req = CRITICAL;
            return 1;
        }
        else if (!strcmp(var_value, "NORMAL"))
        {
            shared_memory->pdu_req = NORMAL;
            return 1;
        }
        else
            return 0;
    }

    return 0;

}

int config_parser(struct _shared_memory* shared_memory)
{
    shared_memory->parser_postbox_size=0;
  
    shared_memory->multiplexer_postbox_size=0;
  
    shared_memory->oam_client_postbox_size=0;
  
    shared_memory->mac_client_postbox_size=0;
  
    shared_memory->control_postbox_size=0;

    /* Open the file for reading */
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    FILE *fp = fopen(FILENAME, "r");

    if (!fp)
    {
        fprintf(stderr, "Error opening file '%s'\n", FILENAME);
        return 0;
    }

    /* Get the first line of the file. */
    line_size = getline(&line_buf, &line_buf_size, fp);

    /* Loop through until we are done with the file. */
    while (line_size >= 0)
    {
        /* Increment our line count */
        line_count++;

        char var_name[50] = {0};
        char var_value[50] = {0};
        int done = 0, i = 0, j = 0, k = 0;


        for (i = 0; i < line_size - 1; i++)
        {
            if (line_buf[i] == '#')
                break;
            else if (line_buf[i] == '=')
            {
                done++;
                continue;
            }
            else if (line_buf[i] == ' ' || line_buf[i] == '\t')
                continue;

            if (!done)
            {
                var_name[j] = line_buf[i];
                j++;
            }
            else
            {
                var_value[k] = line_buf[i];
                k++;
            }
        }

        /* Show the line details */
        if (i >= 3 && done && var_name[0] != '\0' && var_value[0] != '\0')
        {

            printf("%s = %s\n", var_name, var_value);
            if (!set_var(var_name, var_value, shared_memory))
            {
                printf("Configuration Failed Due to Wrong variable/value: %s = %s", var_name, var_value);
                return 0;
            }
        }
        /* Get the next line */
        line_size = getline(&line_buf, &line_buf_size, fp);
    }

    /* Free the allocated line buffer */
    free(line_buf);
    line_buf = NULL;

    /* Close the file now that we are done with it */
    fclose(fp);

    return 1;
}
