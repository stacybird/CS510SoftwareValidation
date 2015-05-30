public class PhoneNumberField {
    /** The default max length for this field's contents */
    private static final int DEFAULT_MAX_LENGTH = 16;

    /** The complete contents of the field */
    String contents;

    /** The maximum length of the field contents including characters such as whitespace */
    int max_length;

    /** The parsed digits of the phone number possibly contained in this field */
    int[] digits;

    /** Any extension provided in the entered phone number */
    int[] ext;


    /**
     * Returns a string representation of the digits of the entered phone number
     * @return
     */
    public String getPhoneNumber() {
        return formatPhoneNumber(this.digits, this.ext);
    }

    /**
     * Returns a string representation of provided phone digits and extension where the
     * format of the phone number is dependent on the number of digits provided.
     * This method assumes all values in digit[] and ext[] are between 0 and 9 inclusive.
     *
     * @param digits the array of digits for the phone number
     * @param ext the array of digits for the extension, if any
     * @throws IllegalStateException if digits[] is not 0, 7, 10, or 11 in length
     * @return a string representation of digits and ext in the format: +1 (800) 555-1234 ext. 54321
     */
    public static String formatPhoneNumber(int[] digits, int[] ext) throws IllegalStateException {
        int size = digits.length;
        String formattedPhone = "";
        switch(size) {
            case 11:
                formattedPhone += "+";
                formattedPhone += digits[size-11];
                formattedPhone += " ";
            case 10:
                formattedPhone += "(";
                formattedPhone += digits[size-10];
                formattedPhone += digits[size-9];
                formattedPhone += digits[size-8];
                formattedPhone += ") ";
            case 7:
                formattedPhone += digits[size-7];
                formattedPhone += digits[size-6];
                formattedPhone += digits[size-5];
                formattedPhone += "-";
                formattedPhone += digits[size-4];
                formattedPhone += digits[size-3];
                formattedPhone += digits[size-2];
                formattedPhone += digits[size-1];
            case 0: break;
            default: throw new IllegalStateException("Parsed phone number is invalid length.");
        }
        if(ext.length >0) {
            formattedPhone += " ext. ";
            for (int i = 0; i < ext.length; i++) formattedPhone += ext[i];
        }
        return formattedPhone;
    }


    /**
     * Triggers the Field to attempt to parse a phone number from the contents of the Field.
     * If the contents do not contain 7, 10, or 11 digits then a phone number is not found,
     * where digits that appear after an 'x' or "ext" do not count towards this total.
     * No parsing occurs if alphabetical characters are found, with the exception of a single
     * instance of 'x' or "ext". Punctuation is ignored.
     * Once a phone number is parsed, the contents of the field are replaced by the formatted
     * representation of the parsed phone number digits and any provided extension.
     * @return
     */
    public PhoneNumberField parseContentsForPhoneNumber() {
        //TODO
        return this;
    }



    ///////////////////// CONSTRUCTORS ////////////////////

    /**
     * Creates an empty PhoneNumberField
     */
    public PhoneNumberField() {
        contents = "";
        max_length = DEFAULT_MAX_LENGTH;
        digits = new int[11]; //1-800-555-1234 standard phone number format
        ext = new int[0];
    }

    /**
     * Creates a PhoneNumberField that is prepopulated with information that is not required to contain
     * a phone number; this is to allow the user to label the Field beforehand.
     * @param prepopulate
     */
    public PhoneNumberField(String prepopulate) {
        contents = prepopulate;
        max_length = DEFAULT_MAX_LENGTH;
        digits = new int[11];
        ext = new int[0];
    }

    /**
     * Creates a PhoneNumberField that may be prepopulated with information, and with a customized maximum
     * length either to accomodate the prepopulated information or to limit the size of the entered
     * phone number (such as if a country code or area code was not necessary).
     * @param prepopulate
     * @param lengthOverride
     */
    public PhoneNumberField(String prepopulate, int lengthOverride) {
        contents = prepopulate;
        max_length = lengthOverride;
        digits = new int[11];
        ext = new int[0];
    }
}
